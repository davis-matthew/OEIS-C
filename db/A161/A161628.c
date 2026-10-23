/*
 * A161628  E.g.f.: A(x,y) = LambertW(x*y*exp(x))/(x*y*exp(x)), as a triangle
 *          of coefficients T(n,k) = [x^n*y^k/n! ] A(x,y), read by rows.
 * https://oeis.org/A161628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2 and T(r,k) = (-1)^k * binomial(r,k) * (k+1)^(k-1) *
 *            k^(r-k). This is the row‑major flattening of the OEIS triangle
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 60, i64 n <= 145,
 *            i128 n <= 370, big any n
 * Helpers:   binom_n_k, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom_n_k.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
       Since n is small (≤ 10⁴), a simple loop is sufficient. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n) r++;
    r--;                               /* r is now the correct row index */
    idx_t offset = (r * (r + 1)) / 2;   /* first index of this row */
    idx_t k = n - offset;              /* column within the row */

    /* Special case for the first element (row 0, column 0). */
    if (r == 0 && k == 0) return (seq_t)1;

    /* For k == 0 and r > 0, the triangle entry is 0. */
    if (k == 0) return (seq_t)0;

    /* Compute the binomial coefficient and the powers. */
    seq_t binom = binom_n_k(r, k);
    seq_t pow1  = seq_pow((seq_t)(k + 1), k - 1);  /* (k+1)^(k-1) */
    seq_t pow2  = seq_pow((seq_t)k, r - k);        /* k^(r-k) */

    /* Multiply the factors. */
    seq_t result = binom;
    result *= pow1;
    result *= pow2;

    /* Apply the sign (-1)^k. */
    if (k & 1) result = -result;

    return result;
}
