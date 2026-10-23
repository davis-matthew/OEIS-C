/*
 * A156550  a(n) = 5*(-1)^n*A078008(n).
 * https://oeis.org/A156550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = (5*(2+(-2)^n))/3
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 28, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute (-2)^n: 2^n with sign */
    seq_t power = pow2(n);          /* power = 2^n */
    if (n & 1) {                    /* if n is odd, negate */
        power = -power;
    }
    /* a(n) = 5 * (2 + power) / 3 */
    seq_t result = ((seq_t)5 * ((seq_t)2 + power)) / 3;
    return result;
}
