/*
 * A224191  Number of 4Xn 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of 4×n arrays with entries 0..2, columns
 *            non‑decreasing, rows unimodal
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 73, u128 n <= 210,
 *            big any n
 * Helpers:   _build_columns, _build_trans
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/_build_columns.h"
#include "../utils/_build_trans.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    static int col[15][4];
    static int16_t trans[1296][15];
    static bool built = false;

    if (!built) {
        _build_columns(col);
        _build_trans(col, trans);
        built = true;
    }

    /* dp[s] = number of ways to reach state s after current column */
    seq_t dp[1296] = {0};
    seq_t dp_next[1296];

    /* initial column: every admissible column can be first */
    for (int t = 0; t < 15; ++t) {
        int s = ((col[t][0] << 1) | 1) + 6*((col[t][1] << 1) | 1)
              + 36*((col[t][2] << 1) | 1) + 216*((col[t][3] << 1) | 1);
        dp[s] = dp[s] + 1;
    }

    /* process remaining columns */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 1296; ++i) dp_next[i] = 0;
        for (int s = 0; s < 1296; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;
            for (int t = 0; t < 15; ++t) {
                int ns = trans[s][t];
                if (ns >= 0)
                    dp_next[ns] = dp_next[ns] + ways;
            }
        }
        for (int i = 0; i < 1296; ++i) dp[i] = dp_next[i];
    }

    seq_t result = 0;
    for (int s = 0; s < 1296; ++s) result = result + dp[s];
    return result;
}
