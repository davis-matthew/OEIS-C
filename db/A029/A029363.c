/*
 * A029363  Expansion of 1/((1 - x^4)*(1 - x^7)*(1 - x^9)*(1 - x^10)).
 * https://oeis.org/A029363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 7, 9, and 10
 *            Computation: dynamic programming, counting each way to make n
 *            using the given coin denominations
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array for values 0..n */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return 0; /* allocation failure – in practice this should not happen */

    /* Initialize all to zero and set base case */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Coin denominations */
    const idx_t coins[4] = {4, 7, 9, 10};

    /* DP: for each coin, update counts for all amounts >= coin */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
