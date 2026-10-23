/*
 * A029277  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^6)*(1-x^12)).
 * https://oeis.org/A029277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 734, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coin change DP for parts 3,5,6,12:
   a(n) = number of non‑negative integer solutions of
          3*a + 5*b + 6*c + 12*d = n */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate dp[0..n] */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;               /* allocation failure – unlikely */

    /* initialise dp[0] = 1, others = 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* coin denominations */
    const idx_t parts[4] = {3, 5, 6, 12};

    /* unbounded coin change DP */
    for (int p_idx = 0; p_idx < 4; ++p_idx) {
        idx_t p = parts[p_idx];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
