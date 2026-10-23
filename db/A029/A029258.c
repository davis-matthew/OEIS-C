/*
 * A029258  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 4, 7, and 9
 * Range:     u8 n <= 93, u16 n <= 655, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a table of size n+1 */
    idx_t size = n + 1;
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp)
        abort();

    /* Initialise dp[0] = 1, the rest 0 */
    for (idx_t i = 0; i < size; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Unbounded knapsack DP: for each coin, add contributions to larger sums */
    const idx_t coins[4] = {3, 4, 7, 9};
    for (int k = 0; k < 4; ++k) {
        idx_t coin = coins[k];
        for (idx_t i = coin; i < size; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
