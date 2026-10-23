/*
 * A029041  Expansion of 1/((1-x)*(1-x^3)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A029041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 5 and 9
 *            (coin‑change counting). The function computes the coefficient of
 *            x^n in 1/((1‑x)(1‑x³)(1‑x⁵)(1‑x⁹))
 * Range:     u8 n <= 50, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t parts[4] = {1, 3, 5, 9};

seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) abort();

    /* Initialise dp[0] = 1, others = 0 */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Classic coin‑change DP for combinations (order does not matter). */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        if (p > n) continue;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
