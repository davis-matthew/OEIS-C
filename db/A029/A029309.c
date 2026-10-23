/*
 * A029309  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of partitions of n using parts 3, 7, 8, and
 *            12. Computed by an unbounded‑knapsack DP. a(0)=1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
#ifdef __cplusplus
    seq_t *dp = new seq_t[(size_t)n + 1];
#else
    seq_t *dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
#endif

    size_t len = (size_t)n + 1;
    dp[0] = 1;
    for (size_t i = 1; i < len; ++i)
        dp[i] = 0;

    const idx_t parts[4] = {3, 7, 8, 12};
    for (int pi = 0; pi < 4; ++pi)
    {
        idx_t part = parts[pi];
        if (part > n) continue;
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[(size_t)n];

    /* Deallocate */
#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif
    return result;
}
