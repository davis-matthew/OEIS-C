/*
 * A131556  Period 6: repeat [1, -2, 1, -1, 2, -1].
 * https://oeis.org/A131556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = [1, -2, 1, -1, 2, -1] repeating with period 6, a(0)=1
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, -2, 1, -1, 2, -1};
    return pattern[(int)(n % 6)];
}
