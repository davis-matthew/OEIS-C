/*
 * A183858  a(n) = n + floor(sqrt(3n)).
 * https://oeis.org/A183858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(3*n))
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(3*n)) using the utility floor_sqrt.
       All arithmetic is performed in seq_t to avoid overflow. */
    seq_t mul3 = (seq_t)n * 3;
    seq_t sqr = floor_sqrt(mul3);
    return (seq_t)n + sqr;
}
