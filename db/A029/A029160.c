/*
 * A029160  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A029160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 7, and 12.
 *            Computed by dynamic programming (unlimited coin change)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* If n is zero, there is exactly one way: use no parts. */
    if (n == 0)
        return (seq_t)1;

    /* Coins available. */
    const idx_t coins[] = {2, 3, 7, 12};
    const size_t k = 4;

    /* Allocate DP table of size n+1. */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return (seq_t)0; /* allocation failure: unlikely in test harness */

    /* Initialize table. */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Main DP: for each coin, add contributions to reachable sums. */
    for (size_t ci = 0; ci < k; ++ci) {
        idx_t c = coins[ci];
        for (idx_t s = c; s <= n; ++s)
            dp[s] += dp[s - c];
    }

    /* Result is number of ways to form n. */
    seq_t result = dp[n];
    free(dp);
    return result;
}
