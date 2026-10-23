/*
 * A131193  Period 6: repeat [0, 1, -3, 3, -1, 0].
 * https://oeis.org/A131193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [0, 1, -3, 3, -1, 0][n mod 6]
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {0, 1, -3, 3, -1, 0};
    return pattern[(size_t)(n % 6)];
}
