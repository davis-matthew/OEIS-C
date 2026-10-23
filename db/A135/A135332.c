/*
 * A135332  a(1)=1; for n>1, a(n) = a(n-1) + n^0 if n odd, a(n) = a(n-1) + n^3
 *          if n is even.
 * https://oeis.org/A135332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n/2) + 8 * ( floor(n/2) * (floor(n/2)+1) / 2
 *            )^2 for n >= 1; a(1) = 1. Recurrence: a(1)=1; for n>1, a(n) =
 *            a(n-1) + 1 if n odd, + n^3 if n even
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 429, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n / 2;

    /* t = m*(m+1)/2  (sum of first m integers) */
    seq_t t = (seq_t)m * ((seq_t)(m + 1)) / 2;

    /* t2 = t^2  (sum of cubes factor before the 8) */
    seq_t t2 = t * t;

    /* sum_even_cubes = 8 * t2 */
    seq_t sum_even_cubes = (seq_t)8 * t2;

    /* a(n) = n - m + sum_even_cubes */
    seq_t result = (seq_t)n - (seq_t)m + sum_even_cubes;

    return result;
}
