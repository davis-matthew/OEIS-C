/*
 * A008312  Triangle of coefficients of Chebyshev polynomials U_n(x).
 * https://oeis.org/A008312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy, nice
 * Formula:   a(n) = coefficient of the n‑th term in the triangle of non‑zero
 *            coefficients of Chebyshev polynomials U_n(x), linearized row by
 *            row from low to high degree
 * Range:     i8 n <= 17, i16 n <= 59, i32 n <= 203, i64 n <= 718,
 *            i128 n <= 2600, big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the nth term. */
    idx_t sum = 0;
    idx_t r   = 0;
    for ( ; ; ++r )
    {
        idx_t len = (r / 2) + 1;       /* number of non‑zero coeffs in row r */
        if (sum + len > n)
            break;
        sum += len;
    }

    idx_t pos  = n - sum;              /* position within row r (0‑based) */
    idx_t half = r / 2;                /* floor(r/2)                */
    idx_t k    = half - pos;           /* index of binomial term    */
    idx_t exp  = (r & 1) + 2 * pos;    /* power of two in coefficient */

    seq_t coef = binom(r - k, k);      /* binomial coefficient      */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)exp);  /* 2^exp      */
    coef *= pow2;                     /* combine to get coefficient */
    if (k & 1)                        /* apply the (-1)^k sign     */
        coef = -coef;

    return coef;
}
