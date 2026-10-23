/*
 * A164662  Row sums of triangle A164658 (numerators of coefficients from
 *          Integral_{x} T(n,x), with T(n,x) Chebyshev polynomials of the
 *          first kind).
 * https://oeis.org/A164662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{m=1..n+1} A164658(n,m) where A164658(n,m) is the
 *            numerator of the reduced coefficient of x^m in the integral of
 *            the Chebyshev polynomial T_n(x). The coefficients of T_n are:
 *            T_n(x) = Σ_{j=0}^{⌊n/2⌋} (-1)^j * 2^{n-2j-1} * n/(n-j) *
 *            binomial(n-j, j) * x^{n-2j} The integral coefficient o...
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 26, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   binomial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Computes the nth term of sequence A164662.
   idx_t is an unsigned integer type (uint64_t),
   seq_t is a signed integer type or arbitrary‑precision integer. */
seq_t a(idx_t n)
{
    /* Special case n == 0: T_0(x) = 1,  ∫T_0 = x,  a(0) = 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t maxj = n / 2;          /* floor(n/2) */
    for (idx_t j = 0; j <= maxj; ++j)
    {
        idx_t k   = n - 2 * j;   /* exponent of x in T_n */
        idx_t d   = k + 1;       /* denominator when integrating */
        int   exp = (int)n - (int)(2 * j) - 1;   /* exponent of 2 in coeff */

        /* coefficient of x^k in T_n */
        seq_t coeff = binomial(n - j, j);   /* C(n-j, j) */
        coeff *= (seq_t)n;                 /* multiply by n   */
        coeff /= (seq_t)(n - j);           /* divide by (n-j) */

        if (exp >= 0)
            coeff *= pow2((idx_t)exp);    /* multiply by 2^exp   */
        else
            coeff /= pow2((idx_t)(-exp)); /* divide by 2^{-exp}  */

        if (j & 1)                        /* apply (-1)^j sign   */
            coeff = -coeff;

        /* Divide by gcd(|coeff|, d) to get reduced numerator */
        seq_t abs_coeff = coeff;
        if (abs_coeff < (seq_t)0)
            abs_coeff = -abs_coeff;
        seq_t g = gcd(abs_coeff, (seq_t)d);
        sum += coeff / g;
    }
    return sum;
}
