/*
 * A228767  Second bisection of the inverse binomial transform of the rational
 *          sequence with e.g.f. (x/2)*(exp(-x)+1)/(exp(x)-1).
 * https://oeis.org/A228767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (8+4^n)*(1-2*n)/8 for n>1 , a(1) = -2
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer form:  a(n) = -(2*n-1)*(1+2^(2*n-3))  for n>1 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)-2;

    /* compute 2^(2*n-3) */
    idx_t exp = 2 * n - 3;                 /* 2*n may still fit in idx_t (uint64_t) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)exp);

    /* (1 + 2^(2*n-3)) */
    seq_t val = pow2 + (seq_t)1;

    /* (2*n - 1) */
    seq_t coeff = (seq_t)2 * (seq_t)n - (seq_t)1;

    /* result = -(2*n-1) * (1 + 2^(2*n-3)) */
    return -coeff * val;
}
