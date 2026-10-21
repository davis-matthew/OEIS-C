/*
 * A061288  Integer part of square root of n-th triangular number.
 * https://oeis.org/A061288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n*(n+1)/2))
 * Range:     u8 n <= 10, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute the n-th triangular number in the seq_t type */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    return floor_sqrt(tri);
}
