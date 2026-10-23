/*
 * A151575  G.f.: (1+x)/(1+x-2*x^2).
 * https://oeis.org/A151575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2 + (-2)^n)/3
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);          /* p = 2^n  */
    if ((n & 1) == 0) {        /* n even: (2 + 2^n)/3  */
        return (seq_t)(2 + p) / 3;
    } else {                   /* n odd:  (2 - 2^n)/3  */
        return (seq_t)(2 - p) / 3;
    }
}
