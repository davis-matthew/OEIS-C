/*
 * A079628  Array of coefficients of P(n,x) = det (M(n,x)) where M(n,x) is the
 *          n X n matrix m(i,j)=x if i>j m(i,j)=1-x if i<=j.
 * https://oeis.org/A079628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coeff of x^k in (-1)^r (x-1)(2x-1)^(r-1) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r(r+1)/2
 * Range:     i8 n <= 23, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Determine the row r and column k (0‑based) in the triangular array. */
    idx_t r = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) * 0.5L);
    /* Correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;

    /* Special case: 0×0 matrix (row 0).  The determinant is 1. */
    if (r == 0) return (seq_t)1;

    /* Coefficient of (2x-1)^(r-1) at x^j follows
       coeff_j = coeff_{j-1} * ( (r-1-(j-1)) * 2 ) * (-1) / j   (for j>0)   */
    seq_t coeff_prev = 0;
    seq_t coeff_curr;

    /* coeff_0 = (-1)^(r-1)  (since 2^0 = 1) */
    coeff_curr = (((r - 1) & 1) ? -(seq_t)1 : (seq_t)1);

    if (k == 0) {
        /* coeff_prev is 0, coeff_curr is coeff_0 */
    } else {
        /* Compute coefficients up to degree k.
           If k > r-1 the coefficient beyond the highest degree is 0. */
        for (idx_t j = 1; j <= k; ++j) {
            if (j > r - 1) {          /* beyond the polynomial's degree */
                coeff_prev = coeff_curr;
                coeff_curr = 0;
                break;
            }
            seq_t next = coeff_curr *
                         (seq_t)(r - 1 - (j - 1)) * (seq_t)2;
            next = -next;            /* alternation of the (-1) term */
            next = next / (seq_t)j;   /* exact division */
            coeff_prev = coeff_curr;
            coeff_curr = next;
        }
    }

    /* Final coefficient: (-1)^r * (coeff_{k-1} - coeff_k) */
    seq_t sign = ((r & 1) ? -(seq_t)1 : (seq_t)1);
    return sign * (coeff_prev - coeff_curr);
}
