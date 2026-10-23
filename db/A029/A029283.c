/*
 * A029283  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 8, and 10
 * Range:     u8 n <= 109, u16 n <= 765, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a temporary array for dynamic programming */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (dp == NULL)
        return 0; /* Allocation failure – unlikely for n ≤ 10000 */

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Weights of the allowed parts */
    static const idx_t w[4] = {3, 5, 8, 10};

    /* Coin‑change DP: count partitions */
    for (int k = 0; k < 4; ++k)
    {
        idx_t weight = w[k];
        for (idx_t i = weight; i <= n; ++i)
            dp[i] = dp[i] + dp[i - weight];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
