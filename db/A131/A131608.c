/*
 * A131608  Period 24: repeat 1, 1, 3, 7, 7, 1, 9, 9, 7, 3, 3, 9, -1, -1, -3,
 *          -7, -7, -1, -9, -9, -7, -3, -3, -9.
 * https://oeis.org/A131608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -a(n-12) (period 24)
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic sequence of period 24 */
    static const seq_t seq[24] = {
        1, 1, 3, 7, 7, 1, 9, 9,
        7, 3, 3, 9, -1, -1, -3, -7,
        -7, -1, -9, -9, -7, -3, -3, -9
    };
    return seq[(size_t)(n % 24)];
}
