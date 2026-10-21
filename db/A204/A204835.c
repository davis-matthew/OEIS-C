/*
 * A204835  Number of (n+1)X4 0..2 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204835: Number of (n+1)×4 arrays over {0,1,2}
 * with all 2×2 permanents equal and nonzero.
 * Computed by dynamic programming over rows of length 4. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Build the lookup tables once. */
    static bool initialized = false;
    static int rows[81][4];
    static int neighborCnt[9][81];
    static int neighbors[9][81][81];

    if (!initialized) {
        /* enumerate all 3⁴ = 81 rows */
        int r = 0;
        for (int a = 0; a <= 2; ++a)
            for (int b = 0; b <= 2; ++b)
                for (int c = 0; c <= 2; ++c)
                    for (int d = 0; d <= 2; ++d) {
                        rows[r][0] = a;
                        rows[r][1] = b;
                        rows[r][2] = c;
                        rows[r][3] = d;
                        ++r;
                    }

        /* build adjacency lists for each permanent value K=1..8 */
        for (int i = 0; i < 81; ++i) {
            for (int j = 0; j < 81; ++j) {
                int p0 = rows[i][0] * rows[j][1] + rows[i][1] * rows[j][0];
                int p1 = rows[i][1] * rows[j][2] + rows[i][2] * rows[j][1];
                int p2 = rows[i][2] * rows[j][3] + rows[i][3] * rows[j][2];
                if (p0 == p1 && p1 == p2 && p0 > 0) {
                    int k = p0;          /* 1..8 */
                    int idx = neighborCnt[k][i]++;
                    neighbors[k][i][idx] = j;
                }
            }
        }
        initialized = true;
    }

    seq_t answer = 0;

    /* Perform DP for each permanent value K. */
    for (int K = 1; K <= 8; ++K) {
        seq_t cur[81], nxt[81];
        for (int i = 0; i < 81; ++i) cur[i] = 1;   /* start with any first row */

        /* n steps → array has n+1 rows */
        for (idx_t step = 0; step < n; ++step) {
            for (int i = 0; i < 81; ++i) nxt[i] = 0;
            for (int r = 0; r < 81; ++r) {
                if (cur[r] == 0) continue;                /* skip zero counts */
                int deg = neighborCnt[K][r];
                for (int e = 0; e < deg; ++e) {
                    int s = neighbors[K][r][e];
                    nxt[s] = nxt[s] + cur[r];
                }
            }
            for (int i = 0; i < 81; ++i) cur[i] = nxt[i];
        }

        /* sum all counts for this K */
        seq_t sumK = 0;
        for (int i = 0; i < 81; ++i) sumK = sumK + cur[i];
        answer = answer + sumK;
    }

    return answer;
}
