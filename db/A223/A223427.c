/*
 * A223427  5X5X5 triangular graph without horizontal edges coloring a
 *          rectangular array: number of nX3 0..14 arrays where 0..14 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 3,6 3,7 4,7
 *          4,8 5,8 5,9 6,10 6,11 7,11 7,12 8,12 8,13 9,13 9,14 and every
 *          array movement to a horizontal or vertical neigh...
 * https://oeis.org/A223427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* /OEIS/A223427
 * Sequence: Number of  n×3 arrays of labels 0..14 that form a graph homomorphism
 * to the 15‑vertex graph with edges  
 * 0‑1, 0‑2, 1‑3, 1‑4, 2‑4, 2‑5, 3‑6, 3‑7, 4‑7, 4‑8, 5‑8, 5‑9,
 * 6‑10, 6‑11, 7‑11, 7‑12, 8‑12, 8‑13, 9‑13, 9‑14.  
 *
 * For n = 1 the value counts 3‑step walks in the graph: 120.
 * For general n the value is the number of 3×n arrays that are
 * successive column states compatible with the graph.
 *
 * The implementation uses dynamic programming with a transfer matrix.
 * Complexity: O(n · S·avg_out) with S = 120 states, avg_out ≈ 80;  
 * well under the time limit for n ≤ 10 000.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 15
#define MAX_STATES 3375   /* 15^3, upper bound for number of column states */

struct State {
    uint8_t r[3];        /* labels in the column (top, middle, bottom) */
};

static bool initialized = false;
static int S;                          /* number of valid column states */
static struct State states[MAX_STATES];/* array of states */
static int outdeg[MAX_STATES];          /* number of outgoing transitions for each state */
static int *trans_index = NULL;         /* start index in trans_list for each state */
static int *trans_list  = NULL;         /* flattened list of destination state indices */

/* Build the adjacency matrix and states once */
static void init(void) {
    bool G[MAX_NODES][MAX_NODES] = {0};
    /* edges of the graph */
    const int edges[][2] = {
        {0,1},{0,2},{1,3},{1,4},{2,4},{2,5},
        {3,6},{3,7},{4,7},{4,8},{5,8},{5,9},
        {6,10},{6,11},{7,11},{7,12},{8,12},{8,13},
        {9,13},{9,14}
    };
    const int n_edges = sizeof(edges)/sizeof(edges[0]);

    /* fill adjacency (undirected) */
    for (int i = 0; i < n_edges; ++i) {
        int u = edges[i][0];
        int v = edges[i][1];
        G[u][v] = G[v][u] = true;
    }

    /* build neighbor lists for quick enumeration of directed edges */
    int neigh[MAX_NODES][MAX_NODES];
    int neigh_count[MAX_NODES] = {0};

    for (int u = 0; u < MAX_NODES; ++u) {
        for (int v = 0; v < MAX_NODES; ++v) {
            if (G[u][v]) {
                neigh[u][neigh_count[u]++] = v;
            }
        }
    }

    /* enumerate all vertical‑valid column states (a,b,c) with a–b and b–c edges */
    S = 0;
    for (int a = 0; a < MAX_NODES; ++a) {
        for (int ia = 0; ia < neigh_count[a]; ++ia) {
            int b = neigh[a][ia];
            for (int ib = 0; ib < neigh_count[b]; ++ib) {
                int c = neigh[b][ib];
                states[S].r[0] = (uint8_t)a;
                states[S].r[1] = (uint8_t)b;
                states[S].r[2] = (uint8_t)c;
                ++S;
            }
        }
    }

    /* compute outgoing transitions for each state */
    int total_trans = 0;
    for (int i = 0; i < S; ++i) {
        int cnt = 0;
        for (int j = 0; j < S; ++j) {
            if (G[states[i].r[0]][states[j].r[0]] &&
                G[states[i].r[1]][states[j].r[1]] &&
                G[states[i].r[2]][states[j].r[2]]) {
                ++cnt;
            }
        }
        outdeg[i] = cnt;
        total_trans += cnt;
    }

    /* allocate transition list */
    trans_index = (int*)malloc((S + 1) * sizeof(int));
    trans_list  = (int*)malloc(total_trans * sizeof(int));

    trans_index[0] = 0;
    int pos = 0;
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            if (G[states[i].r[0]][states[j].r[0]] &&
                G[states[i].r[1]][states[j].r[1]] &&
                G[states[i].r[2]][states[j].r[2]]) {
                trans_list[pos++] = j;
            }
        }
        trans_index[i + 1] = pos;
    }

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (!initialized) init();

    /* handle n=0 (off‑offset) – not used */
    if (n == 0) return (seq_t)0;

    /* allocate two temporary arrays of counts */
    seq_t *cnt = (seq_t*)calloc(S, sizeof(seq_t));
    seq_t *nxt = (seq_t*)calloc(S, sizeof(seq_t));

    /* initial column can be any vertical‑valid state */
    for (int i = 0; i < S; ++i) cnt[i] = (seq_t)1;

    /* iterate over columns (n-1 transitions) */
    for (idx_t step = 1; step < n; ++step) {
        /* zero the next array */
        memset(nxt, 0, S * sizeof(seq_t));

        /* propagate counts */
        for (int i = 0; i < S; ++i) {
            seq_t val = cnt[i];
            int start = trans_index[i];
            int end   = trans_index[i + 1];
            for (int p = start; p < end; ++p) {
                nxt[ trans_list[p] ] += val;
            }
        }

        /* swap arrays */
        seq_t *tmp = cnt;
        cnt = nxt;
        nxt = tmp;
    }

    /* sum all counts for the final column */
    seq_t result = (seq_t)0;
    for (int i = 0; i < S; ++i) {
        result += cnt[i];
    }

    /* free temporaries */
    free(cnt);
    free(nxt);

    return result;
}
