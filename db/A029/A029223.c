/*
 * A029223  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   number of partitions of n into parts 2, 6, 9, and 12 (OEIS
 *            A029223). We compute the ordinary generating‑function
 *            1/((1-x^2)(1-x^6)(1-x^9)(1-x^12)) by a standard coin‑change DP.
 *            dp[i] holds the number of ways to form i using the available
 *            coins
 * Range:     u8 n <= 107, u16 n <= 779, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t */
#include "../utils/seq_alloc.h"    /* seq_alloc */

seq_t a(idx_t n) {
    /* static cache to avoid reallocating on every call */
    static seq_t *dp = NULL;
    static size_t dp_size = 0;
    static idx_t dp_max = 0;  /* largest n already handled */

    /* grow the cache if necessary */
    if ((size_t)n + 1 > dp_size) {
        dp = seq_alloc((size_t)n + 1);
        dp_size = (size_t)n + 1;
    }

    /* initialise dp[0..n] */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* coin denominations */
    static const idx_t coins[] = {2, 6, 9, 12};
    const size_t coin_cnt = sizeof(coins)/sizeof(coins[0]);

    /* unbounded knapsack DP: coin outer loop, ensures unordered partitions */
    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t c = coins[ci];
        for (idx_t j = c; j <= n; ++j)
            dp[j] += dp[j - c];
    }

    dp_max = n;
    return dp[n];
}
