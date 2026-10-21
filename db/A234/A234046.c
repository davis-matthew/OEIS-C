/*
 * A234046  Period 7: repeat [0, 1, -1, 0, 0, -1, 1].
 * https://oeis.org/A234046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = pattern[n mod 7] where pattern = [0, 1, -1, 0, 0, -1, 1]
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t period[7] = {
    (seq_t)0,
    (seq_t)1,
    (seq_t)-1,
    (seq_t)0,
    (seq_t)0,
    (seq_t)-1,
    (seq_t)1
};

seq_t a(idx_t n)
{
    /* Return the value corresponding to n modulo 7 */
    return period[(size_t)(n % 7u)];
}
