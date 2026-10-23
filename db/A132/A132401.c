/*
 * A132401  Period 8: repeat 0, 0, 1, 1, 2, -1, -1, -2.
 * https://oeis.org/A132401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = pattern[n mod 8] where pattern = {0,0,1,1,2,-1,-1,-2}
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[8] = { 0, 0, 1, 1, 2, -1, -1, -2 };
    return period[(size_t)(n & 7U)];
}
