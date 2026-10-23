/*
 * A029229  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ partitions of n into parts 2, 7, 8, 12 }
 * Range:     u8 n <= 109, u16 n <= 791, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Unbounded coin change DP, works for any idx_t and seq_t defined by the build.
   Allocates an array of length n+1, iteratively adds contributions of each part.
*/
#include <stdlib.h>

#if defined(__cplusplus)
#  include <new>
#endif

seq_t a(idx_t n)
{
    /* Coin denominations */
    static const idx_t parts[4] = {2, 7, 8, 12};

    /* Allocate DP array of size n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp;
#if defined(__cplusplus)
    dp = new seq_t[len];                      /* ctor invoked for big‑int types */
#else
    dp = (seq_t*)malloc(len * sizeof(seq_t)); /* mem zero‑init later if needed */
#endif
    if (!dp) return (seq_t)0;                 /* graceful failure on OOM */

    /* Initialise dp[0] = 1, others 0 */
    for (size_t i = 0; i < len; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Unbounded coin change (order of loops gives combinations) */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t coin = parts[pi];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
