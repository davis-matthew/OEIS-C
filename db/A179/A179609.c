/*
 * A179609  a(n)=(5-(-1)^n-6*n)*2^(n-2).
 * https://oeis.org/A179609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (5-(-1)^n-6*n)*2^(n-2)
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 25, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle the two exceptional cases where n-2 would be negative */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t sign   = (n & 1) ? (seq_t)-1 : (seq_t)1;       /* -1 if n odd, +1 if n even */
    seq_t coeff  = (seq_t)5 - sign - (seq_t)6 * n;        /* 5 - (-1)^n - 6*n */
    seq_t pow    = pow2(n - 2);                          /* 2^(n-2) */
    return coeff * pow;
}
