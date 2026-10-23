/*
 * A137694  Numbers k such that 6k^2-2k = 3n^2-n for some integer n>0.
 * https://oeis.org/A137694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = first component of f^(n-1)(5,7) where f(x,y) = (577*x +
 *            408*y - 164, 816*x + 577*y - 232)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS definition. Zero or negative indices are unused. */
    seq_t x = 5;
    seq_t y = 7;

    /* Apply the affine transformation f exactly (n-1) times. */
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t nx = 577 * x + 408 * y - (seq_t)164;
        seq_t ny = 816 * x + 577 * y - (seq_t)232;
        x = nx;
        y = ny;
    }

    return x;   /* a(n) is the first coordinate. */
}
