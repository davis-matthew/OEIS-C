/*
 * A131720  Period 6: repeat [0, 1, -1, 1, -1, 0].
 * https://oeis.org/A131720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = pattern[n mod 6] where pattern = [0, 1, -1, 1, -1, 0]
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {0, 1, -1, 1, -1, 0};
    return pattern[(size_t)(n % 6)];
}
