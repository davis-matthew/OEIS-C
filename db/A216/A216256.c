/*
 * A216256  Minimum length of a longest unimodal subsequence of a permutation
 *          of n elements.
 * https://oeis.org/A216256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((sqrt(12*n-3) - 1) / 2)
 * Range:     u8 n <= 5, u16 n <= 1003, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimum length of a longest unimodal subsequence of a permutation of n elements (OEIS A216256). */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1; for safety we handle n==0, returning 0 */
    if (n == 0)
        return (seq_t)0;

    /* Work in seq_t to avoid overflow for large n.
       m = 12*n - 3,   a(n) = ceil((sqrt(m) - 1) / 2)            */
    seq_t m = (seq_t)12 * (seq_t)n - (seq_t)3;

    /* s = floor(sqrt(m)) */
    seq_t s = floor_sqrt(m);

    /* t = floor((s-1)/2)   */
    seq_t t = (s - (seq_t)1) / (seq_t)2;

    /* If m is a perfect square, the fractional part is zero,
     * so the ceil is exactly t. Otherwise add 1. */
    seq_t sq = s * s;
    if (sq == m)
        return t;
    else
        return t + (seq_t)1;
}
