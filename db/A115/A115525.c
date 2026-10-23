/*
 * A115525  Periodic {1,1,-2,0,1,0,-1,0,0,1,-1,-1}.
 * https://oeis.org/A115525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = periodic sequence modulo 12: a(n) = [1, 1, -2, 0, 1, 0,
 *            -1, 0, 0, 1, -1, -1][n mod 12]
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[12] = {1, 1, -2, 0, 1, 0, -1, 0, 0, 1, -1, -1};
    return period[(size_t)(n % 12)];
}
