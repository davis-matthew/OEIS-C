/*
 * A203105  Number of n X 5 binary arrays with every 1 immediately preceded by
 *          0 0 to the left or above.
 * https://oeis.org/A203105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×5 binary arrays with every 1 immediately preceded by
 *            0 0 to the left or above. Computed by dynamic programming over
 *            rows in a 5‑column grid
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   fully_valid_mask, hor_valid_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fully_valid_mask.h"
#include "../utils/hor_valid_mask.h"

/* Main function required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1; value for n=0 is undefined */
    /* DP tables: dp_prev[prev0][prev1], dp_cur[prev0][prev1] */
    seq_t dp_prev[32][32] = {0}, dp_cur[32][32] = {0};
    /* Precompute horizontal valid masks */
    bool hor_valid[32];
    for (int m = 0; m < 32; ++m) hor_valid[m] = hor_valid_mask((uint8_t)m);
    /* Row 1 */
    for (int m = 0; m < 32; ++m) {
        if (hor_valid[m]) dp_prev[0][m] = 1;
    }
    if (n == 1) { /* answer is sum over dp_prev */
        seq_t sum = 0;
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                sum += dp_prev[i][j];
        return sum;
    }
    /* Row 2 */
    for (int prev0 = 0; prev0 < 32; ++prev0)
        for (int prev1 = 0; prev1 < 32; ++prev1)
            if (dp_prev[prev0][prev1]) {
                seq_t cnt = dp_prev[prev0][prev1];
                for (int m = 0; m < 32; ++m) {
                    if (hor_valid[m]) dp_cur[prev1][m] += cnt;
                }
            }
    /* copy dp_cur to dp_prev for next iterations */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp_prev[i][j] = dp_cur[i][j];
    if (n == 2) {
        seq_t sum = 0;
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                sum += dp_prev[i][j];
        return sum;
    }
    /* Rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_cur[i][j] = 0;  /* reset current DP */
        for (int prev0 = 0; prev0 < 32; ++prev0) {
            for (int prev1 = 0; prev1 < 32; ++prev1) {
                seq_t base = dp_prev[prev0][prev1];
                if (!base) continue;
                for (int m = 0; m < 32; ++m) {
                    if (fully_valid_mask((uint8_t)prev0, (uint8_t)prev1, (uint8_t)m)) {
                        dp_cur[prev1][m] += base;
                    }
                }
            }
        }
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_prev[i][j] = dp_cur[i][j];
    }
    /* final sum */
    seq_t result = 0;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            result += dp_prev[i][j];
    return result;
}
