/*
 * A179962  Number of permutations p of [2*n+4] such that |p(i)-p(i-1)| > n
 *          for i>1.
 * https://oeis.org/A179962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (49/2)*4^n - (8/3)*3^n (a(0)=24)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;

    /* 4^n and 3^n computed with fast exponentiation */
    seq_t pow4 = pow_uint(4, n);          /* 4^n */
    seq_t term1 = (((seq_t)49 * pow4) / 2);  /* (49/2)*4^n */

    seq_t pow3 = pow_uint(3, n);          /* 3^n */
    seq_t term2 = (((seq_t)8 * pow3) / 3);   /* (8/3)*3^n */

    return term1 - term2;
}
