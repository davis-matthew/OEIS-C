/*
 * A175921  Period 5: repeat [1, 2, 2, -1, 1].
 * https://oeis.org/A175921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = pattern[(n-1) mod 5], pattern = { 1, 2, 2, -1, 1 }
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[5] = { 1, 2, 2, -1, 1 };
    return pat[(size_t)((n - 1) % 5)];
}
