/*
 * A131289  Period 12: repeat 1, 1, 3, -3, -3, 1, -1, -1, -3, 3, 3, -1.
 * https://oeis.org/A131289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = repeat 1, 1, 3, -3, -3, 1, -1, -1, -3, 3, 3, -1 (period
 *            12)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[12] = {
        1, 1, 3, -3, -3, 1,
       -1, -1, -3, 3, 3, -1
    };
    return period[(size_t)(n % (idx_t)12)];
}
