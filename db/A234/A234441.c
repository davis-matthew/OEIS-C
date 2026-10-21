/*
 * A234441  Number of (n+1) X (6+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+1)×7 0..2 arrays such that every 2×2
 *            subblock has its diagonal sum differing from its antidiagonal
 *            sum by exactly 1. Computed by transfer‑matrix DP over row
 *            patterns
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#define STATES 2187           /* 3^7 possible row patterns */
#define MAX_DEG  70          /* maximum degree of any state (safety margin) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe return */

    /* Static data for adjacency and DP buffers (built once) */
    static int rows[STATES][7];
    static int adj[STATES][MAX_DEG];
    static int deg[STATES];
    static seq_t buf1[STATES], buf2[STATES];
    static seq_t *cur = NULL, *next = NULL;
    static int initialized = 0;

    if (!initialized) {
        /* Enumerate all row patterns (base‑3 representation) */
        for (int s = 0; s < STATES; ++s) {
            int v = s;
            for (int c = 6; c >= 0; --c) {
                rows[s][c] = v % 3;
                v /= 3;
            }
        }

        /* Build adjacency: undirected, stored bidirectionally */
        for (int i = 0; i < STATES; ++i) deg[i] = 0;
        for (int r = 0; r < STATES; ++r) {
            for (int s = r + 1; s < STATES; ++s) {
                int ok = 1;
                for (int i = 0; i < 6; ++i) {
                    int diff = (rows[r][i] + rows[s][i + 1])
                               - (rows[r][i + 1] + rows[s][i]);
                    if (diff < 0) diff = -diff;
                    if (diff != 1) { ok = 0; break; }
                }
                if (ok) {
                    adj[r][deg[r]++] = s;
                    adj[s][deg[s]++] = r;
                }
            }
        }

        cur  = buf1;
        next = buf2;
        initialized = 1;
    }

    /* Initialize DP: all rows are possible starting rows */
    for (int i = 0; i < STATES; ++i) cur[i] = 1;

    /* Iterate over n steps (edges) */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero next buffer */
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        /* Transition */
        for (int r = 0; r < STATES; ++r) {
            seq_t val = cur[r];
            if (val == 0) continue;
            int d = deg[r];
            int *nb = adj[r];
            for (int k = 0; k < d; ++k)
                next[nb[k]] += val;
        }

        /* Swap buffers */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all counts: total number of arrays */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i) result += cur[i];
    return result;
}
