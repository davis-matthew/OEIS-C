/*
 * A069309  Number of 5 X n binary arrays with a path of adjacent 1's from
 *          upper left corner to anywhere in right hand column.
 * https://oeis.org/A069309
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of 5×n binary arrays with a path of adjacent 1's from the
   upper‑left corner to any cell in the rightmost column (OEIS A069309).
   Computation is performed by dynamic programming over columns: each state
   records the connectivity of the current frontier (five rows) and which
   component contains the start.  States are compactly encoded as a 18‑bit
   key and combined when identical.  The DP runs in O(n·S·32), where S ≤ 260
   is the number of distinct states. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t comp[5];   /* component id per row (0–4) or 5 for empty */
    uint8_t start_id;  /* component id that contains the start cell */
    seq_t   count;     /* number of partial arrays leading to this state */
} State;

/* Union‑Find helpers */
static inline int uf_find(int *parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}
static inline void uf_union(int *parent, int a, int b) {
    a = uf_find(parent, a);
    b = uf_find(parent, b);
    if (a != b) parent[b] = a;
}

/* Encode a state into a 18‑bit key for quick comparison. */
static inline uint32_t state_key(const State *s) {
    uint32_t key = 0;
    for (int i = 0; i < 5; ++i) key = (key << 3) | (s->comp[i] & 7);
    key = (key << 3) | (s->start_id & 7);
    return key;
}

/* Build the initial state for the first column given its pattern. */
static void init_state(uint8_t pat, State *out) {
    int node[5], parent[10], cnt = 0;
    /* Assign a node to every 1 in the pattern */
    for (int r = 0; r < 5; ++r) {
        if (pat & (1u << r)) {
            node[r] = cnt;
            parent[cnt] = cnt;
            ++cnt;
        } else {
            node[r] = -1;
        }
    }
    /* Vertical merges inside the column */
    for (int r = 0; r < 4; ++r) {
        if ((pat & (1u << r)) && (pat & (1u << (r + 1))))
            uf_union(parent, node[r], node[r + 1]);
    }
    /* Assign new component IDs */
    int root_to_id[10];
    for (int i = 0; i < 10; ++i) root_to_id[i] = -1;
    int next_id = 0;
    for (int r = 0; r < 5; ++r) {
        if (pat & (1u << r)) {
            int root = uf_find(parent, node[r]);
            if (root_to_id[root] == -1) root_to_id[root] = next_id++;
            out->comp[r] = root_to_id[root];
        } else {
            out->comp[r] = 5;
        }
    }
    /* The start component is the one containing row 0 (which must be 1). */
    out->start_id = out->comp[0];
    out->count   = 1;
}

/* Transition from a previous state to a new column pattern.  Returns 1 if
   the start component survives; otherwise returns 0. */
static int transition(const State *prev, uint8_t pat, State *out) {
    /* Map existing components to nodes if they appear in this column. */
    int comp_to_node[5];
    for (int i = 0; i < 5; ++i) comp_to_node[i] = -1;

    int node_of_row[5];
    int node_count = 0;

    int start_present = 0;          /* flag if the start component appears */
    int start_row = -1;             /* first row that belongs to start */

    for (int r = 0; r < 5; ++r) {
        if (pat & (1u << r)) {
            if (prev->comp[r] != 5) {
                int c = prev->comp[r];
                if (comp_to_node[c] == -1) comp_to_node[c] = node_count++;
                node_of_row[r] = comp_to_node[c];
            } else {
                node_of_row[r] = node_count++;
            }
            if (prev->comp[r] == prev->start_id) {
                start_present = 1;
                start_row = r;
            }
        } else {
            node_of_row[r] = -1;
        }
    }
    if (!start_present) return 0;   /* start component vanished */

    int parent[10];
    for (int i = 0; i < node_count; ++i) parent[i] = i;

    /* Vertical merges inside the column */
    for (int r = 0; r < 4; ++r) {
        if ((pat & (1u << r)) && (pat & (1u << (r + 1))))
            uf_union(parent, node_of_row[r], node_of_row[r + 1]);
    }

    /* Identify root of the start component after merges */
    int start_root = uf_find(parent, node_of_row[start_row]);

    /* Map roots to compact component IDs */
    int root_to_id[10];
    for (int i = 0; i < 10; ++i) root_to_id[i] = -1;
    int next_id = 0;
    for (int r = 0; r < 5; ++r) {
        if (pat & (1u << r)) {
            int root = uf_find(parent, node_of_row[r]);
            if (root_to_id[root] == -1) root_to_id[root] = next_id++;
            out->comp[r] = root_to_id[root];
        } else {
            out->comp[r] = 5;
        }
    }
    out->start_id = root_to_id[start_root];
    return 1;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n < 2) return 0;  /* undefined below the offset */

    State cur[512], nxt[512];
    size_t cur_sz = 0, nxt_sz = 0;

    /* Initialize first column. */
    for (uint8_t pat = 0; pat < 32u; ++pat) {
        if ((pat & 1u) == 0) continue; /* upper-left corner must be 1 */
        State s;
        init_state(pat, &s);
        cur[cur_sz++] = s;
    }

    /* Process remaining columns. */
    for (idx_t col = 1; col < n; ++col) {
        nxt_sz = 0;
        for (size_t i = 0; i < cur_sz; ++i) {
            const State *prev = &cur[i];
            for (uint8_t pat = 0; pat < 32u; ++pat) {
                State tmp;
                if (!transition(prev, pat, &tmp)) continue;  /* start died */
                uint32_t key = state_key(&tmp);
                size_t j;
                for (j = 0; j < nxt_sz; ++j) {
                    if (state_key(&nxt[j]) == key) break;
                }
                if (j == nxt_sz) {
                    tmp.count = prev->count;
                    nxt[nxt_sz++] = tmp;
                } else {
                    nxt[j].count += prev->count;
                }
            }
        }
        /* Prepare for next iteration. */
        for (size_t i = 0; i < nxt_sz; ++i) cur[i] = nxt[i];
        cur_sz = nxt_sz;
    }

    /* Sum counts over all final states (the start component reaches the last column). */
    seq_t total = 0;
    for (size_t i = 0; i < cur_sz; ++i) total += cur[i].count;
    return total;
}
