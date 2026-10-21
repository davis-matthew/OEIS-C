/*
 * A223428  5X5X5 triangular graph without horizontal edges coloring a
 *          rectangular array: number of nX4 0..14 arrays where 0..14 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 3,6 3,7 4,7
 *          4,8 5,8 5,9 6,10 6,11 7,11 7,12 8,12 8,13 9,13 9,14 and every
 *          array movement to a horizontal or vertical neigh...
 * https://oeis.org/A223428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A223428 – Number of n×4 0..14 arrays that are graph-homomorphisms
 *  from the 4‑column grid to the graph with nodes 0..14 and edges
 *  0‑1 0‑2 1‑3 1‑4 2‑4 2‑5 3‑6 3‑7 4‑7 4‑8 5‑8 5‑9 6‑10 6‑11 7‑11 7‑12 8‑12 8‑13 9‑13 9‑14
 *  This implementation uses a transfer‑matrix DP over the
 *  admissible horizontal 4‑tuples as states.  For each row it
 *  propagates counts by multiplying with the vertical‑adjacency
 *  matrix.  The adjacency lists are precomputed once and reused
 *  for all calls.  The algorithm runs in O(#states²) for
 *  the preprocessing and O(n·#states·avg_deg) per requested
 *  n, which is well below the 2‑second limit for n≤10 000.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- constants ---------------------------------------------------- */
#define NODES 15                          /* graph nodes 0 … 14 */
#define STATE_BASE 15                     /* base for encoding a 4‑tuple */
#define STATE_SIZE (STATE_BASE*STATE_BASE*STATE_BASE*STATE_BASE) /* 15⁴ = 50625 */

/* adjacency information --------------------------------------------------- */
static const int adj_deg[NODES] = {
    2,3,3,3,4,3,3,4,4,3,1,2,2,2,1
};

static const int adj_list[NODES][4] = {
    /* 0 */ {1,2,0,0},
    /* 1 */ {0,3,4,0},
    /* 2 */ {0,4,5,0},
    /* 3 */ {1,6,7,0},
    /* 4 */ {1,2,7,8},
    /* 5 */ {2,8,9,0},
    /* 6 */ {3,10,11,0},
    /* 7 */ {3,4,11,12},
    /* 8 */ {4,5,12,13},
    /* 9 */ {5,13,14,0},
    /*10 */ {6,0,0,0},
    /*11 */ {6,7,0,0},
    /*12 */ {7,8,0,0},
    /*13 */ {8,9,0,0},
    /*14 */ {9,0,0,0}
};

/* horizontal edge matrix – true iff {a,b} is an edge */
static const bool edge_matrix[NODES][NODES] = {
    /* 0 */ {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    /* 1 */ {1,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
    /* 2 */ {1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
    /* 3 */ {0,1,0,0,0,0,1,1,0,0,0,0,0,0,0},
    /* 4 */ {0,1,1,0,0,0,0,1,1,0,0,0,0,0,0},
    /* 5 */ {0,0,1,0,0,0,0,0,1,1,0,0,0,0,0},
    /* 6 */ {0,0,0,1,0,0,0,0,0,0,1,1,0,0,0},
    /* 7 */ {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0},
    /* 8 */ {0,0,0,0,1,1,0,0,0,0,0,0,1,1,0},
    /* 9 */ {0,0,0,0,0,1,0,0,0,0,0,0,0,1,1},
    /*10 */ {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    /*11 */ {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    /*12 */ {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
    /*13 */ {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
    /*14 */ {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0}
};

/* ---------- helper functions ------------------------------------------------ */

static inline int encode(int a, int b, int c, int d)
{
    return d*3375 + c*225 + b*15 + a;
}

static inline void decode(int code, int out[4])
{
    out[3] = code / 3375;   code %= 3375;
    out[2] = code / 225;    code %= 225;
    out[1] = code / 15;
    out[0] = code % 15;
}

/* ---------- global tables and state --------------------------------------- */
static bool init_done = false;

static int  valid_states[STATE_SIZE];
static int  valid_cnt = 0;
static int  state_to_idx[STATE_SIZE];

struct neighbor_list {
    int cnt;
    int list[256];                 /* upper bound: 4⁴ */
};

static struct neighbor_list *neighbor = NULL;

static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* ---------- build tables ----------------------------------------------------- */
static void build_tables(void)
{
    /* initialise lookup */
    for (int i = 0; i < STATE_SIZE; ++i) state_to_idx[i] = -1;

    /* enumerate all colorings that satisfy horizontal edges */
    int node[4];
    for (int a = 0; a < NODES; ++a) {
        for (int b = 0; b < NODES; ++b) {
            if (!edge_matrix[a][b]) continue;
            for (int c = 0; c < NODES; ++c) {
                if (!edge_matrix[b][c]) continue;
                for (int d = 0; d < NODES; ++d) {
                    if (!edge_matrix[c][d]) continue;
                    int code = encode(a, b, c, d);
                    state_to_idx[code] = valid_cnt;
                    valid_states[valid_cnt++] = code;
                }
            }
        }
    }

    /* allocate neighbor lists */
    neighbor = (struct neighbor_list *)malloc(sizeof(*neighbor) * valid_cnt);
    for (int i = 0; i < valid_cnt; ++i) {
        int cur_nodes[4];
        decode(valid_states[i], cur_nodes);

        int neigh_deg[4];
        int neigh_buf[4][4];
        for (int j = 0; j < 4; ++j) {
            neigh_deg[j] = adj_deg[cur_nodes[j]];
            for (int k = 0; k < neigh_deg[j]; ++k)
                neigh_buf[j][k] = adj_list[cur_nodes[j]][k];
        }

        int cnt = 0;
        for (int n0 = 0; n0 < neigh_deg[0]; ++n0)
        for (int n1 = 0; n1 < neigh_deg[1]; ++n1)
        for (int n2 = 0; n2 < neigh_deg[2]; ++n2)
        for (int n3 = 0; n3 < neigh_deg[3]; ++n3) {
            int code2 = encode(neigh_buf[0][n0], neigh_buf[1][n1],
                               neigh_buf[2][n2], neigh_buf[3][n3]);
            int idx2 = state_to_idx[code2];
            if (idx2 >= 0)
                neighbor[i].list[cnt++] = idx2;
        }
        neighbor[i].cnt = cnt;
    }

    /* DP arrays */
    dp_cur  = (seq_t *)malloc(sizeof(seq_t) * valid_cnt);
    dp_next = (seq_t *)malloc(sizeof(seq_t) * valid_cnt);
    init_done = true;
}

/* ---------- public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) build_tables();

    if (n == 0) return (seq_t)0;           /* undefined below offset */

    /* first row – all admissible states have weight 1 */
    for (int i = 0; i < valid_cnt; ++i) dp_cur[i] = (seq_t)1;

    /* propagate rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < valid_cnt; ++i) dp_next[i] = (seq_t)0;
        for (int i = 0; i < valid_cnt; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0) continue;
            int *nb = neighbor[i].list;
            int nb_cnt = neighbor[i].cnt;
            for (int j = 0; j < nb_cnt; ++j)
                dp_next[nb[j]] += val;
        }
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum over all ending states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < valid_cnt; ++i)
        result += dp_cur[i];
    return result;
}
