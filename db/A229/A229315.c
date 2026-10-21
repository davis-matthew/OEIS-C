/*
 * A229315  Number of nX2 0..3 arrays of the median of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1)X3 0..3 array without adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays over {0,…,3} which are the median of
 *            a corresponding (n+1)×3 array with no equal adjacent cells; the
 *            value is obtained by dynamic programming on a DFA that
 *            recognises all valid (n+1)×3 arrays
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* The types seq_t and idx_t are provided by the build system.  They
   may be 8‑, 16‑, 32‑, 64‑ or 128‑bit unsigned integers, or an
   arbitrary‑precision integer class.  The implementation uses only
   the operations permitted by the specification. */

/* -------------------------------------------------------------------- */
/* Data structures used internally.                                     */
/* -------------------------------------------------------------------- */
struct State {
    uint64_t mask;                /* subset of the 36 valid rows of the   */
                                  /* larger array (bit i set → row i allowed) */
    int next[16];                 /* transition on each possible median  */
                                   /* (00..00  (0) … 15  (1111))          */
};

struct Automaton {
    int S;                /* number of DFA states          */
    struct State *states;
    int idx_start;        /* start state (all rows allowed) */
    int idx_empty;        /* state with empty mask, if any */
    seq_t *dp_cur;        /* distribution after current step */
    seq_t *dp_next;       /* temporary distribution          */
};

static struct Automaton *autoP = NULL;   /* global automaton              */
static seq_t *seq_cache  = NULL;         /* memoised sequence values      */
static size_t cache_cap  = 0;
static idx_t cache_n     = 0;

/* -------------------------------------------------------------------- */
/* Helper – median of three 0..3 integers.                               */
/* -------------------------------------------------------------------- */
static inline int median3(int x, int y, int z)
{
    int minv = (x < y) ? x : y;  if (z < minv) minv = z;
    int maxv = (x > y) ? x : y;  if (z > maxv) maxv = z;
    return x + y + z - minv - maxv;
}

/* -------------------------------------------------------------------- */
/* Build the deterministic finite automaton recognizing all valid (n+1)×3 arrays,
   then allocate the DP tables.                                           */
/* -------------------------------------------------------------------- */
static void build_automaton(void)
{
    /* Allocate the automaton struct.  The cast is required for C++.        */
    struct Automaton *autoG = (struct Automaton *)malloc(sizeof(struct Automaton));
    autoP = autoG;

    /* 1. Enumerate all 36 rows with no equal adjacent values.             */
    int rows[36][3];
    int row_code_to_idx[64];
    memset(row_code_to_idx, -1, sizeof(row_code_to_idx));

    int idx = 0;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                if (a != b && b != c) {
                    rows[idx][0] = a;
                    rows[idx][1] = b;
                    rows[idx][2] = c;
                    int code = (a << 4) | (b << 2) | c;
                    row_code_to_idx[code] = idx;
                    ++idx;
                }
    int Nrows = idx;   /* should be 36 */

    /* 2. Pre‑compute transitions between rows for each median letter.   */
    uint64_t trans_mask[36][16];
    memset(trans_mask, 0, sizeof(trans_mask));

    for (int i = 0; i < Nrows; ++i)
        for (int j = 0; j < Nrows; ++j) {
            if (rows[i][0] == rows[j][0] ||
                rows[i][1] == rows[j][1] ||
                rows[i][2] == rows[j][2])
                continue;           /* vertical adjacency disallowed */
            int m0 = median3(rows[i][0], rows[i][1], rows[j][0]);
            int m1 = median3(rows[i][1], rows[i][2], rows[j][1]);
            int l  = (m0 << 2) | m1;          /* 0..15                */
            trans_mask[i][l] |= (1ULL << j);
        }

    /* 3. BFS over subsets to build DFA states.                           */
    int cap = 1024;               /* initial capacity for states     */
    struct State *states = (struct State *)malloc(cap * sizeof(struct State));
    int *queue = (int *)malloc(cap * sizeof(int));
    int q_head = 0, q_tail = 0;

    /* initial state: all rows allowed */
    uint64_t all_mask = (1ULL << 36) - 1ULL;
    states[0].mask = all_mask;
    for (int l = 0; l < 16; ++l) states[0].next[l] = -1;
    int state_cnt = 1;
    queue[q_tail++] = 0;          /* enqueue start state */

    while (q_head < q_tail) {
        int cur = queue[q_head++];
        uint64_t cur_mask = states[cur].mask;

        for (int l = 0; l < 16; ++l) {
            uint64_t next_mask = 0ULL;
            uint64_t bits = cur_mask;
            while (bits) {
                int irow = __builtin_ctzll(bits);
                next_mask |= trans_mask[irow][l];
                bits &= bits - 1;
            }

            /* find existing state or create a new one */
            int next_idx = -1;
            for (int t = 0; t < state_cnt; ++t)
                if (states[t].mask == next_mask) { next_idx = t; break; }

            if (next_idx == -1) {
                if (state_cnt == cap) {
                    cap <<= 1;
                    states = (struct State *)realloc(states, cap * sizeof(struct State));
                    queue  = (int *)realloc(queue,  cap * sizeof(int));
                }
                next_idx = state_cnt;
                states[next_idx].mask = next_mask;
                for (int ll = 0; ll < 16; ++ll) states[next_idx].next[ll] = -1;
                queue[q_tail++] = next_idx;
                ++state_cnt;
            }
            states[cur].next[l] = next_idx;
        }
    }

    free(queue);   /* queue no longer needed */

    /* 4. Store automaton information. */
    autoG->S            = state_cnt;
    autoG->states       = states;
    autoG->idx_start    = 0;                /* initial state */
    autoG->idx_empty    = -1;
    for (int i = 0; i < state_cnt; ++i)
        if (states[i].mask == 0ULL) { autoG->idx_empty = i; break; }

    /* 5. Allocate DP tables. */
    autoG->dp_cur  = (seq_t *)calloc(state_cnt, sizeof(seq_t));
    autoG->dp_next = (seq_t *)calloc(state_cnt, sizeof(seq_t));
    autoG->dp_cur[autoG->idx_start] = (seq_t)1;   /* start distribution */

    /* 6. Allocate cache for sequence values. */
    cache_cap = 1;
    seq_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));  /* index 0 unused */
    cache_n   = 0;
}

/* -------------------------------------------------------------------- */
/* Ensure the cache contains values up to index n.                           */
/* -------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (!autoP) build_automaton();

    if (n <= cache_n) return;                /* already computed */

    /* grow the cache if necessary */
    while (cache_cap <= n) {
        cache_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
    }

    /* iterate from the current maximum to the new n */
    for (idx_t step = cache_n; step < n; ++step)
    {
        /* dp_next = 0 */
        memset(autoP->dp_next, 0, autoP->S * sizeof(seq_t));

        /* propagate one step */
        for (int u = 0; u < autoP->S; ++u) {
            seq_t val = autoP->dp_cur[u];
            if (val == (seq_t)0) continue;
            const int *next = autoP->states[u].next;
            for (int l = 0; l < 16; ++l) {
                int v = next[l];
                autoP->dp_next[v] += val;
            }
        }

        /* swap current and next */
        seq_t *tmp = autoP->dp_cur;
        autoP->dp_cur = autoP->dp_next;
        autoP->dp_next = tmp;

        /* compute a(step+1) = sum of all non‑empty states */
        seq_t sum = (seq_t)0;
        for (int v = 0; v < autoP->S; ++v)
            if (v != autoP->idx_empty)
                sum += autoP->dp_cur[v];
        seq_cache[step + 1] = sum;
    }
    cache_n = n;
}

/* -------------------------------------------------------------------- */
/* Public function required by the specification                           */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, harmless     */
    ensure_cache(n);
    return seq_cache[n];
}
