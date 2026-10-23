/*
 * A029272  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 6, and 7
 * Range:     u8 n <= 88, u16 n <= 617, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array dp[0..n] */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* allocation failed */

    dp[0] = 1;                    /* base: one way to sum to 0 */

    /* Parts allowed in the partition */
    static const idx_t parts[4] = {3, 5, 6, 7};

    for (size_t pi = 0; pi < 4; ++pi)
    {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
