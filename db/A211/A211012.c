/*
 * A211012  Total area of all squares and rectangles after 2^n stages in the
 *          toothpick structure of A139250, assuming the toothpicks have
 *          length 2.
 * https://oeis.org/A211012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (2^n - 2) (a(0) = 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t t = pow2(n);        /* t = 2^n */
    return t * (t - (seq_t)2);
}
