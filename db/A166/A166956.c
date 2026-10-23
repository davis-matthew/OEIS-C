/*
 * A166956  a(n) = 2^n +(-1)^n - 2.
 * https://oeis.org/A166956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n + (-1)^n - 2
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);
    if ((n & 1) == 0) {
        return v - 1;
    } else {
        return v - 3;
    }
}
