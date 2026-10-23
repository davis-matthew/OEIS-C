/*
 * A171983  Beatty sequence for sqrt(13).
 * https://oeis.org/A171983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(13))
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* Compute a(n) = floor(n * sqrt(13)) without floating point.
       n >= 1.  The value is floor(sqrt(13 * n * n)). */
    seq_t nn = (seq_t)n;
    seq_t prod = (seq_t)13 * nn * nn;
    return sqrt_floor(prod);
}
