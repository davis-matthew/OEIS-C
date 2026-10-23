/*
 * A154589  a(n) = A154570(n) + A154570(n+1).
 * https://oeis.org/A154589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 4; for n >= 1: a(n) = -2^(n-1) (i.e. a(n+1) =
 *            -A000079(n))
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)4;
    }
    seq_t v = pow2(n - 1);   /* 2^(n-1) */
    return -v;
}
