/*
 * A029135  Expansion of 1/((1-x)(1-x^10)(1-x^11)(1-x^12)).
 * https://oeis.org/A029135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x)(1-x^10)(1-x^11)(1-x^12))
 *            equivalently, the number of non‑negative integer solutions to i
 *            + 10*j + 11*k + 12*l = n
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate array for dynamic programming up to n. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;                 /* allocation failed, return 0 */

    /* Initialize dp table */
    dp[0] = 1;
    for (idx_t d = 1; d <= n; ++d) {
        dp[d] = 0;
    }

    /* Coin sizes: 1, 10, 11, 12 */
    static const idx_t parts[4] = {1, 10, 11, 12};
    for (size_t pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t d = p; d <= n; ++d) {
            dp[d] += dp[d - p];
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
