/*
 * A069448  Half the number of 3 X n binary arrays with no path of adjacent
 *          1's or adjacent 0's from top to bottom or side to side.
 * https://oeis.org/A069448
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of 3×n binary arrays with no 0‑ or 1‑path
 *            connecting top–bottom or left–right) / 2 (OEIS A069448)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   encode_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/encode_state.h"

/* --------------------------------------------------------------------- */
/*  State representation and hash table                                 */
/* --------------------------------------------------------------------- */

#define MAX_STATES   8192          /* generous upper bound for the 3×n case */
#define HASH_SIZE    8192
#define HASH_MASK    (HASH_SIZE-1)

/* Component 4‑bit flags: 0x1 top, 0x2 bottom, 0x4 left */
typedef struct {
    uint8_t bits;          /* column value (3 bits) */
    uint8_t comp[3];       /* component id for each row (0..2) */
    uint8_t flags[3];      /* flags for each component (up to 3) */
    uint8_t k;             /* number of components in this column (≤3) */
    bool   left_present;   /* true if at least one component touches left   */
} StateInfo;

static StateInfo *states          = NULL;
static int       *trans           = NULL;  /* trans[state*8 + bits] = next_state */
static uint32_t  *hash_keys       = NULL;
static int       *hash_vals       = NULL;
static int        *pattern_state   = NULL; /* initial state id for each 3‑bit pattern */
static int        *queue           = NULL;

static int state_count            = 0;
static bool   initialized        = false;

/* forward reference for helper find/unite */
static inline int uf_find(int p[], int x);
static inline void uf_unite(int p[], int a, int b);

/* --------------------------------------------------------------------- */
/*  Key generation for hashing                                           */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Union–Find helpers                                                   */
/* --------------------------------------------------------------------- */

static inline int uf_find(int p[], int x)
{
    int root = x;
    while (p[root] != root) {
        root = p[root];
    }
    while (p[x] != root) {
        int next = p[x];
        p[x] = root;
        x = next;
    }
    return root;
}

static inline void uf_unite(int p[], int a, int b)
{
    a = uf_find(p, a);
    b = uf_find(p, b);
    if (a != b) p[b] = a;
}

/* --------------------------------------------------------------------- */
/*  Hash table functions                                                 */
/* --------------------------------------------------------------------- */

static int state_hash_get(uint32_t key)
{
    int h = (int)((key * 2654435761u) & HASH_MASK);
    while (true) {
        if (hash_keys[h] == 0xFFFFFFFFu) return -1;
        if (hash_keys[h] == key) return hash_vals[h];
        h = (h + 1) & HASH_MASK;
    }
}

static void state_hash_put(uint32_t key, int id)
{
    int h = (int)((key * 2654435761u) & HASH_MASK);
    while (hash_keys[h] != 0xFFFFFFFFu) h = (h + 1) & HASH_MASK;
    hash_keys[h] = key;
    hash_vals[h] = id;
}

/* --------------------------------------------------------------------- */
/*  Adding a new state to the global table                               */
/* --------------------------------------------------------------------- */

static int add_state(uint32_t key,
                     uint8_t bits,
                     uint8_t comp[3],
                     uint8_t flags[3],
                     uint8_t k,
                     bool left)
{
    int id = state_hash_get(key);
    if (id != -1) return id;

    if (state_count >= MAX_STATES) {
        /* In practice this should never happen for 3×n arrays. */
        abort();
    }

    id = state_count++;
    states[id].bits = bits;
    states[id].comp[0] = comp[0];
    states[id].comp[1] = comp[1];
    states[id].comp[2] = comp[2];
    states[id].flags[0] = flags[0];
    states[id].flags[1] = flags[1];
    states[id].flags[2] = flags[2];
    states[id].k = k;
    states[id].left_present = left;

    state_hash_put(key, id);
    return id;
}

/* --------------------------------------------------------------------- */
/*  Generate initial state for one column pattern                        */
/* --------------------------------------------------------------------- */

static int generate_initial_state(uint8_t bits)
{
    uint8_t c[3] = {
        (uint8_t)((bits >> 0) & 1),
        (uint8_t)((bits >> 1) & 1),
        (uint8_t)((bits >> 2) & 1)
    };

    /* Union–find among the three cells to detect vertical adjacency */
    int parent[3] = {0, 1, 2};
    for (int i = 0; i < 2; ++i) {
        if (c[i] == c[i + 1]) {
            int ra = uf_find(parent, i);
            int rb = uf_find(parent, i + 1);
            if (ra != rb) parent[rb] = ra;
        }
    }

    /* Assign canonical component IDs (0..k-1) */
    uint8_t comp[3];
    uint8_t id_map[3] = {255, 255, 255};
    uint8_t next_id = 0;
    for (int i = 0; i < 3; ++i) {
        int r = uf_find(parent, i);
        if (id_map[r] == 255) id_map[r] = next_id++;
        comp[i] = id_map[r];
    }

    /* Flags for each component: left, top, bottom */
    uint8_t flags[3] = {0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        uint8_t cid = comp[i];
        flags[cid] |= 4;          /* left edge flag */
        if (i == 0) flags[cid] |= 1;   /* top */
        if (i == 2) flags[cid] |= 2;   /* bottom */
    }

    /* Reject states that already connect top–bottom */
    for (int i = 0; i < next_id; ++i) {
        if ((flags[i] & 3) == 3) return -1;
    }

    bool left = true;  /* all initial components touch left */
    uint32_t key = encode_state(bits, comp, flags);
    return add_state(key, bits, comp, flags, next_id, left);
}

/* --------------------------------------------------------------------- */
/*  Compute transition from a state to a new column pattern              */
/* --------------------------------------------------------------------- */

static int compute_transition(int prev_id, uint8_t next_bits, bool *new_created)
{
    const StateInfo *prev = &states[prev_id];
    int old_state_count = state_count;

    int parent[8];  /* max 6 nodes (3 prev comps + 3 new cells) */
    int N = prev->k + 3;
    for (int i = 0; i < N; ++i) parent[i] = i;

    /* Attach new column cells of same value to previous component */
    for (int i = 0; i < 3; ++i) {
        if ( ((next_bits >> i) & 1) == ((prev->bits >> i) & 1) ) {
            uf_unite(parent, prev->k + i, prev->comp[i]);
        }
    }

    /* Vertical adjacency inside the new column */
    for (int i = 0; i < 2; ++i) {
        if ( ((next_bits >> i) & 1) == ((next_bits >> (i + 1)) & 1) ) {
            uf_unite(parent, prev->k + i, prev->k + i + 1);
        }
    }

    /* Aggregate flags for each root that still has a cell in this column */
    uint8_t root_flags[8] = {0};
    bool used[8] = {false};

    /* previous components */
    for (int i = 0; i < prev->k; ++i) {
        int r = uf_find(parent, i);
        root_flags[r] |= prev->flags[i];
        used[r] = true;
    }

    /* new column cells */
    for (int i = 0; i < 3; ++i) {
        int idx = prev->k + i;
        int r = uf_find(parent, idx);
        if (i == 0) root_flags[r] |= 1;   /* top */
        if (i == 2) root_flags[r] |= 2;   /* bottom */
        used[r] = true;
    }

    /* Reject if any component touches both top and bottom */
    for (int r = 0; r < N; ++r) {
        if (used[r] && ((root_flags[r] & 3) == 3)) {
            *new_created = false;
            return -1;
        }
    }

    /* Assign new component IDs for the 3 cells of this column */
    int root_to_new[8];
    for (int r = 0; r < N; ++r) root_to_new[r] = -1;

    uint8_t new_comp[3] = {0, 0, 0};
    uint8_t new_flags[3] = {0, 0, 0};
    uint8_t new_k = 0;

    for (int i = 0; i < 3; ++i) {
        int idx = prev->k + i;
        int r = uf_find(parent, idx);
        int cid = root_to_new[r];
        if (cid == -1) {
            cid = new_k++;
            root_to_new[r] = cid;
            new_flags[cid] = root_flags[r];
        }
        new_comp[i] = (uint8_t)cid;
    }

    bool left = false;
    for (int i = 0; i < new_k; ++i) {
        if (new_flags[i] & 4) { left = true; break; }
    }

    uint32_t key = encode_state(next_bits, new_comp, new_flags);
    int new_id = add_state(key, next_bits, new_comp, new_flags, new_k, left);
    *new_created = (new_id == old_state_count);
    return new_id;
}

/* --------------------------------------------------------------------- */
/*  Build entire state space (BFS)                                       */
/* --------------------------------------------------------------------- */

static void init(void)
{
    /* Allocate memory */
    states = (StateInfo *)malloc(MAX_STATES * sizeof(StateInfo));
    trans  = (int *)      malloc(MAX_STATES * 8 * sizeof(int));
    hash_keys = (uint32_t *)malloc(HASH_SIZE * sizeof(uint32_t));
    hash_vals = (int *)     malloc(HASH_SIZE * sizeof(int));
    pattern_state = (int *)malloc(8 * sizeof(int));
    queue = (int *)malloc(MAX_STATES * sizeof(int));

    /* Initialise hash table */
    for (int i = 0; i < HASH_SIZE; ++i) hash_keys[i] = 0xFFFFFFFFu;

    /* Build initial column states */
    for (int i = 0; i < 8; ++i) pattern_state[i] = -1;
    for (int bits = 0; bits < 8; ++bits) {
        int id = generate_initial_state((uint8_t)bits);
        if (id != -1) pattern_state[bits] = id;
    }

    /* Initialise transition table */
    for (int i = 0; i < MAX_STATES * 8; ++i) trans[i] = -1;

    /* BFS over reachable states */
    int q_head = 0, q_tail = 0;
    for (int bits = 0; bits < 8; ++bits) {
        if (pattern_state[bits] != -1) queue[q_tail++] = pattern_state[bits];
    }

    while (q_head < q_tail) {
        int cur = queue[q_head++];
        for (int b = 0; b < 8; ++b) {
            bool new_created;
            int nxt = compute_transition(cur, (uint8_t)b, &new_created);
            trans[cur * 8 + b] = nxt;
            if (new_created && nxt != -1) queue[q_tail++] = nxt;
        }
    }

    initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    if (!initialized) init();

    size_t S = (size_t)state_count;
    seq_t *dp  = (seq_t *)malloc(S * sizeof(seq_t));
    seq_t *ndp = (seq_t *)malloc(S * sizeof(seq_t));
    for (size_t i = 0; i < S; ++i) dp[i] = (seq_t)0;

    /* Count initial column arrays */
    for (int bits = 0; bits < 8; ++bits) {
        int id = pattern_state[bits];
        if (id != -1) dp[id] += (seq_t)1;
    }

    /* Process remaining columns */
    for (idx_t col = 2; col <= n; ++col) {
        for (size_t i = 0; i < S; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < (int)state_count; ++s) {
            if (dp[s] == (seq_t)0) continue;
            for (int b = 0; b < 8; ++b) {
                int nxt = trans[s * 8 + b];
                if (nxt != -1) ndp[nxt] += dp[s];
            }
        }
        /* swap dp and ndp */
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
    }

    /* Sum over states that do NOT have a component touching left */
    seq_t total = (seq_t)0;
    for (int s = 0; s < (int)state_count; ++s) {
        if (!states[s].left_present) total += dp[s];
    }

    /* Final required answer is half of total */
    seq_t result = total / (seq_t)2;

    free(dp); free(ndp);
    return result;
}
