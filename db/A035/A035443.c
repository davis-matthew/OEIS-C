/*
 * A035443  Number of partitions of n into parts 8k or 8k+3.
 * https://oeis.org/A035443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k or 8k+3
 * Range:     u8 n <= 80, u16 n <= 195, u32 n <= 545, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate dynamic array for DP */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return 0; /* fallback on allocation failure */

    /* Initialize DP array */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Process parts that are multiples of 8 */
    for (idx_t p = 8; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    /* Process parts congruent to 3 modulo 8 */
    for (idx_t p = 3; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
