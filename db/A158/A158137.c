/*
 * A158137  Period 9: repeat [-2,4,-2,4,-2,-2,1,1,-2].
 * https://oeis.org/A158137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = { -2, 4, -2, 4, -2, -2, 1, 1, -2 } repeated constant with
 *            period 9, offset 0
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[9] = { -2, 4, -2, 4, -2, -2, 1, 1, -2 };
    return pat[(size_t)(n % 9)];
}
