/*
 * A140318  Period 18: repeat 0, 1, -1, 1, 0, -1, 1, -1, 0, 0, -1, 1, -1, 0,
 *          1, -1, 1, 0.
 * https://oeis.org/A140318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) repeats the period 18: {0, 1, -1, 1, 0, -1, 1, -1, 0, 0,
 *            -1, 1, -1, 0, 1, -1, 1, 0}
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[18] = {
        0,  1, -1,  1,
        0, -1,  1, -1,
        0,  0, -1,  1,
       -1,  0,  1, -1,
        1,  0
    };
    return period[(size_t)(n % 18U)];
}
