/*
 * A131372  Period 7: repeat [1, -1, 0, 1, 0, -1, 1].
 * https://oeis.org/A131372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = {1, -1, 0, 1, 0, -1, 1}[ n mod 7 ]
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[7] = {1, -1, 0, 1, 0, -1, 1};
    size_t r = (size_t)(n % 7);
    return pat[r];
}
