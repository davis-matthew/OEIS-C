/*
 * A035966  Number of partitions of n into parts not of the form 17k, 17k+5 or
 *          17k-5. Also number of partitions with at most 4 parts of size 1
 *          and differences between parts at distance 7 are greater than 1.
 * https://oeis.org/A035966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            5, or 12 modulo 17. For n ≥ 1
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate table for DP */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                  /* allocation failed */

    /* base case */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* dynamic programming: iterate over all allowed part sizes */
    for (idx_t part = 1; part <= n; ++part) {
        idx_t r = part % 17;
        if (r == 0 || r == 5 || r == 12)
            continue;                     /* part size forbidden */

        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
