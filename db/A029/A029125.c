/*
 * A029125  Expansion of 1/((1-x)(1-x^7)(1-x^10)(1-x^12)).
 * https://oeis.org/A029125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = number of solutions to a+7b+10c+12d = n with a,b,c,d ≥ 0
   (expansion of 1 / ((1-x)(1-x^7)(1-x^10)(1-x^12))).
   Implementation uses an unbounded 4‑coin knapsack DP. */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1.  For C++ use std::vector, for C use malloc. */
#ifdef __cplusplus
    std::vector<seq_t> dp((size_t)(n + 1));
#else
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;               /* memory failure fallback */
#endif

    /* Initialize dp[0] = 1, others 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Coin denominations: 1, 7, 10, 12 */
    const idx_t coins[4] = {1, 7, 10, 12};

    for (int ci = 0; ci < 4; ++ci) {
        idx_t coin = coins[ci];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];

#ifndef __cplusplus
    free(dp);
#endif
    return result;
}
