/*
 * A035986  Number of partitions of n into parts not of the form 21k, 21k+8 or
 *          21k-8. Also number of partitions with at most 7 parts of size 1
 *          and differences between parts at distance 9 are greater than 1.
 * https://oeis.org/A035986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to *
 *            0, 8, or 13 modulo 21 (i.e., excluding part sizes 21k, 21k+8,
 *            21k-8)
 * Range:     u8 n <= 16, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, free, size_t */
#include <stdint.h>     /* idx_t, seq_t assumed defined in build */

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for the offset 1; return 0 safely */
        return 0;

    /* Allocate DP array for sums 0..n */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;   /* fallback if memory allocation fails */

    /* Initialize */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Partition DP: iterate over allowed part sizes */
    for (idx_t part = 1; part <= n; ++part)
    {
        idx_t mod = part % 21;
        if (mod == 0 || mod == 8 || mod == 13)   /* forbidden part */
            continue;

        for (idx_t sum = part; sum <= n; ++sum)
            dp[sum] += dp[sum - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
