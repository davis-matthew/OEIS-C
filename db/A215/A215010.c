/*
 * A215010  Integer side lengths in arithmetic progression of simple convex
 *          hexagons with equal interior angles. Sequence gives the values of
 *          m for sides of lengths t+m*d, counterclockwise, for the two
 *          primitive solutions.
 * https://oeis.org/A215010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) repeats the pattern of the 12 m-values of the two primitive
 *            hexagon solutions in order, i.e. a(n) = pat[(n-1) mod 12] with
 *            pat = {0,5,1,3,2,4,0,5,2,1,4,3}
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[12] = {0, 5, 1, 3, 2, 4, 0, 5, 2, 1, 4, 3};
    idx_t pos = (n - 1) % 12;
    return pat[(size_t)pos];
}
