/*
 * A223410  4X4X4 triangular graph without horizontal edges coloring a
 *          rectangular array: number of nX3 0..9 arrays where 0..9 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 3,6 3,7 4,7
 *          4,8 5,8 5,9 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 66 The sequence counts the number of n×3 arrays with
 *            entries from 0..9 such that each pair of horizontally or
 *            vertically adjacent cells is connected by one of the following
 *            edges: 0-1, 0-2, 1-3, 1-4, 2-4, 2-5, 3-6, 3-7, 4-7, 4-8, 5-8,
 *            5-9. Let f(n,i) be the number of arrays of height n end...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_PAT 70
#define MAX_GRID 10
#define MAX_NEIGH 70

static void build_graph(bool G[MAX_GRID][MAX_GRID]) {
    for (int i = 0; i < MAX_GRID; ++i)
        for (int j = 0; j < MAX_GRID; ++j)
            G[i][j] = false;
    /* edges */
    G[0][1] = G[1][0] = true;
    G[0][2] = G[2][0] = true;
    G[1][3] = G[3][1] = true;
    G[1][4] = G[4][1] = true;
    G[2][4] = G[4][2] = true;
    G[2][5] = G[5][2] = true;
    G[3][6] = G[6][3] = true;
    G[3][7] = G[7][3] = true;
    G[4][7] = G[7][4] = true;
    G[4][8] = G[8][4] = true;
    G[5][8] = G[8][5] = true;
    G[5][9] = G[9][5] = true;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static unsigned char pat[MAX_PAT][3];
    static int neigh[MAX_PAT][MAX_NEIGH];
    static int neigh_cnt[MAX_PAT];
    static int pat_cnt = 0;

    if (!initialized) {
        /* Build adjacency matrix of the label graph */
        bool G[MAX_GRID][MAX_GRID];
        build_graph(G);

        /* Generate all row patterns (triples) satisfying horizontal edges */
        bool used[MAX_GRID][MAX_GRID][MAX_GRID] = {0};
        for (int b = 0; b < MAX_GRID; ++b) {
            for (int a = 0; a < MAX_GRID; ++a) if (G[a][b]) {
                for (int c = 0; c < MAX_GRID; ++c) if (G[b][c]) {
                    if (!used[a][b][c]) {
                        used[a][b][c] = true;
                        pat[pat_cnt][0] = a;
                        pat[pat_cnt][1] = b;
                        pat[pat_cnt][2] = c;
                        ++pat_cnt;
                    }
                }
            }
        }

        /* Compute vertical compatibility lists */
        for (int i = 0; i < pat_cnt; ++i) {
            neigh_cnt[i] = 0;
            for (int j = 0; j < pat_cnt; ++j) {
                bool ok = true;
                for (int k = 0; k < 3; ++k) {
                    if (!G[pat[i][k]][pat[j][k]]) { ok = false; break; }
                }
                if (ok) neigh[i][neigh_cnt[i]++] = j;
            }
        }

        initialized = true;
    }

    if (n == 0) return (seq_t)0;       /* undefined, but safe */

    /* dp arrays */
    static seq_t dp_cur[MAX_PAT];
    static seq_t dp_next[MAX_PAT];

    for (int i = 0; i < pat_cnt; ++i) dp_cur[i] = (seq_t)1; /* n = 1 */

    if (n == 1) {
        seq_t sum = 0;
        for (int i = 0; i < pat_cnt; ++i) sum += dp_cur[i];
        return sum;
    }

    for (idx_t row = 2; row <= n; ++row) {
        for (int j = 0; j < pat_cnt; ++j) dp_next[j] = (seq_t)0;
        for (int i = 0; i < pat_cnt; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0) continue;
            for (int k = 0; k < neigh_cnt[i]; ++k) {
                int j = neigh[i][k];
                dp_next[j] += val;
            }
        }
        for (int i = 0; i < pat_cnt; ++i) dp_cur[i] = dp_next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < pat_cnt; ++i) total += dp_cur[i];
    return total;
}
