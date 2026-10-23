/*
 * A134430  Period 4: repeat [1, -2, -2, 1].
 * https://oeis.org/A134430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1, -2, -2, 1, 1, -2, -2, 1, … (period 4)
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[4] = {1, -2, -2, 1};
    return tbl[n & 3U];
}
