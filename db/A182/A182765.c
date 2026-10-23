/*
 * A182765  Beatty sequence for (6 + sqrt(2))/4.
 * https://oeis.org/A182765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ((6 + √2)/4) * n ) The formula is evaluated
 *            exactly using only integer arithmetic. It follows that for all
 *            integer n ≥ 1 a(n) can be computed as a(n) = (6*n + floor(
 *            √(2*n*n) )) / 4 where the square root is the integer floor of
 *            the non‑negative integer 2*n*n. No floating point is re...
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index into the seq_t type. */
    seq_t nn = (seq_t)n;

    /* Compute floor(√(2*n*n)) = floor(n*√2). */
    seq_t sq = floor_sqrt( (seq_t)2 * nn * nn );

    /* a(n) = (6*n + sq) / 4, integer division is the floor. */
    return ( (seq_t)6 * nn + sq ) / 4;
}
