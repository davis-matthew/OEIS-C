/*
 * A131017  Period 6: repeat [1, 1, 2, -1, 2, 1].
 * https://oeis.org/A131017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, 1, 2, -1, 2, 1][n mod 6]
 * Range:     i8 n <= 116, i16 n <= 116, i32 n <= 116, i64 n <= 116,
 *            i128 n <= 116, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {1, 1, 2, -1, 2, 1};
    return pat[(size_t)(n % 6)];
}
