/*
 * A171974  Integer part of the height of a regular tetrahedron with edge
 *          length n.
 * https://oeis.org/A171974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(6)/3)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Computes the integer part of the height of a regular tetrahedron with edge length n.
   For  n >= 1 it returns floor( n * sqrt(6) / 3 ). */
seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic consistency. */
    seq_t sn = (seq_t)n;

    /* Compute floor(sqrt(6*n^2)).  Since sqrt(6)*n = sqrt(6*n^2), this gives
       floor(n*sqrt(6)).  floor_sqrt is provided by the utils library. */
    seq_t val = floor_sqrt((seq_t)6 * sn * sn);

    /* Integer division by 3 gives floor(n*sqrt(6)/3). */
    return val / (seq_t)3;
}
