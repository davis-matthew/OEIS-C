/*
 * A141035  Period 10: repeat 0, 0, 4, 2, -2, 4, 2, -4, -4, -2.
 * https://oeis.org/A141035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = periodic with period 10, pattern = [0, 0, 4, 2, -2, 4, 2,
 *            -4, -4, -2]
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic pattern array (size 10) */
    static const seq_t pattern[10] = {
        (seq_t)0,
        (seq_t)0,
        (seq_t)4,
        (seq_t)2,
        (seq_t)-2,
        (seq_t)4,
        (seq_t)2,
        (seq_t)-4,
        (seq_t)-4,
        (seq_t)-2
    };

    size_t idx = (size_t)(n % 10);
    return pattern[idx];
}
