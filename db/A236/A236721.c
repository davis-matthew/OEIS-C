/*
 * A236721  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by counting walks of length n in each of the 7
   subgraphs of the 9×9 adjacency graph defined by the constraint
   on 2×2 subblocks (0..2 arrays). This implementation uses
   dynamic programming with only additions (no multiplication).
   a(1) = 81.
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* build adjacency compressed by the value of E(s,t) */
    static bool initialized = false;
    static uint8_t adjacency[7][9][9];   /* adjacency[E][i][j] = 1 if edge i->j has E==E */

    if (!initialized) {
        for (int i = 0; i < 9; ++i) {
            int x0 = i / 3, x1 = i % 3;     /* top row */
            for (int j = 0; j < 9; ++j) {
                int y0 = j / 3, y1 = j % 3; /* bottom row */
                int a = 0, b = 0, c = 0;    /* counts of 0,1,2 */
                int vals[4] = {x0, x1, y0, y1};
                for (int k = 0; k < 4; ++k) {
                    if (vals[k] == 0) ++a;
                    else if (vals[k] == 1) ++b;
                    else ++c;
                }

                /* helper to extract sorted element at position pos */
                int at_pos = 0;
                if (at_pos < a) at_pos = 0;
                int v0 = (0 < a) ? 0 : ((0 < a + b) ? 1 : 2);
                int v1 = (1 < a) ? 0 : ((1 < a + b) ? 1 : 2);
                int v2 = (2 < a) ? 0 : ((2 < a + b) ? 1 : 2);
                int v3 = (3 < a) ? 0 : ((3 < a + b) ? 1 : 2);

                int E = v3 + v2 + v1 - v0;    /* E ∈ {0,1,2,3,4,5,6} */
                adjacency[E][i][j] = 1;
            }
        }
        initialized = true;
    }

    seq_t ans = 0;
    for (int e = 0; e < 7; ++e) {
        seq_t dp_cur[9], dp_next[9];
        for (int i = 0; i < 9; ++i) dp_cur[i] = 1;   /* start at any row */

        for (idx_t step = 1; step <= n; ++step) {
            for (int j = 0; j < 9; ++j) dp_next[j] = 0;
            for (int i = 0; i < 9; ++i) {
                seq_t val = dp_cur[i];
                if (val == 0) continue;
                for (int j = 0; j < 9; ++j) {
                    if (adjacency[e][i][j]) dp_next[j] += val;
                }
            }
            for (int j = 0; j < 9; ++j) dp_cur[j] = dp_next[j];
        }

        seq_t sum = 0;
        for (int j = 0; j < 9; ++j) sum += dp_cur[j];
        ans += sum;
    }
    return ans;
}
