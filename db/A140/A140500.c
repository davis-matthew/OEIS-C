/*
 * A140500  Period 18: repeat [1, 1, -2, 2, -1, -1, 1, -2, 1, -1, -1, 2, -2,
 *          1, 1, -1, 2, -1].
 * https://oeis.org/A140500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) is periodic with period 18. The pattern is * [1, 1, -2, 2,
 *            -1, -1, 1, -2, 1, -1, -1, 2, -2, 1, 1, -1, 2, -1]. * Thus a(n) =
 *            pattern[n mod 18]
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[18] = {
        1, 1, -2, 2, -1, -1,
        1, -2, 1, -1, -1, 2,
       -2, 1, 1, -1, 2, -1
    };
    return pattern[(size_t)(n % 18)];
}
