/*
 * A131974  Period 8: repeat [1, 2, 3, 4, -5, -4, -3, -2].
 * https://oeis.org/A131974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = [1, 2, 3, 4, -5, -4, -3, -2][n mod 8]
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t period[8] = {
        (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)4,
        (seq_t)-5, (seq_t)-4, (seq_t)-3, (seq_t)-2
    };
    return period[n % 8];
}
