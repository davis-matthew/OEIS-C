/*
 * A008674  Expansion of 1/((1-x)*(1-x^3)*(1-x^5)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A008674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 183, u32 n <= 3129, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008674: number of partitions of n into odd parts ≤ 9.
 * Computation: dynamic programming over the generating function
 *   1/((1‑x)(1‑x³)(1‑x⁵)(1‑x⁷)(1‑x⁹))
 * with parts {1,3,5,7,9}.  Complexity O(n) per call, correct for any
 * call order or n ≥ 0.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate table for 0 … n */
    size_t needed = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!dp) return 0;                     /* out of memory → 0 (non‑negative) */

    /* initialise to 0, then base case */
    for (size_t i = 0; i < needed; ++i) dp[i] = 0;
    dp[0] = 1;

    const int parts[5] = {1, 3, 5, 7, 9};
    for (int p = 0; p < 5; ++p)
    {
        size_t part = (size_t)parts[p];
        for (size_t i = part; i < needed; ++i)
            dp[i] += dp[i - part];
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
