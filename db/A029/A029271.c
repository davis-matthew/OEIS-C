/*
 * A029271  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 839, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A029271 – partitions of n into 3, 4, 11, and 12
    a(n) = number of partitions of n using parts 3, 4, 11 and 12. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    const idx_t coins[4] = { 3, 4, 11, 12 };
    /* Allocate dp[0..n] and initialise to 0 */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failed – return 0 as safe fallback */
    dp[0] = (seq_t)1;           /* one way to make 0 */

    /* Classic coin‑change DP: each coin type is considered once,
       merging unordered combinations automatically. */
    for (int ci = 0; ci < 4; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
