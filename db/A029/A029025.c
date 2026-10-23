/*
 * A029025  Expansion of 1/((1-x)*(1-x^2)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 8 and 9
 *            Computation: DP over the four coin denominations. dp[0] = 1; for
 *            each coin value c in {1,2,8,9}: for i = c; i <= n; ++i) dp[i] +=
 *            dp[i-c];
 * Range:     u8 n <= 50, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Allocate a table for the DP values up to n. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        abort();                /* allocation failed */

    /* Initialise the table */
    idx_t i;
    for (i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Denominations to use */
    static const idx_t coins[4] = {1, 2, 8, 9};

    /* DP loop */
    for (int c = 0; c < 4; ++c)
    {
        idx_t coin = coins[c];
        for (i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
