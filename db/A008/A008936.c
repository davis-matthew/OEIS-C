/*
 * A008936  Expansion of (1 - 2*x -x^4)/(1 - 2*x)^2 in powers of x.
 * https://oeis.org/A008936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n for n<4; a(n) = 2^n - (n-3)*2^(n-4) for n≥4
 * Range:     i8 n <= 7, i16 n <= 19, i32 n <= 31, i64 n <= 61, i128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return pow2(n);

    seq_t p2n   = pow2(n);          /* 2^n      */
    seq_t p2n4  = pow2(n - 4);      /* 2^(n-4)  */
    seq_t coeff = (seq_t)(n - 3);   /* n-3 as seq_t */
    seq_t term  = coeff * p2n4;     /* (n-3)*2^(n-4) */
    return p2n - term;             /* 2^n - (n-3)*2^(n-4) */
}
