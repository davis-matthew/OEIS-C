/*
 * A177971  Triangle of polynomial coefficients: p(x,n) = (1 - 2x)^(n + 1)*(1
 *          + Sum_{k>=1} 2^(k - 1)*k^n*x^k).
 * https://oeis.org/A177971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 47, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binom, pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A177971 – Triangle of polynomial coefficients
 *
 * a(n) is the n‑th coefficient in the concatenation of the rows
 * of the polynomials
 *
 *     p(x,n) = (1-2x)^(n+1) * ( 1 + Σ_{k≥1} 2^(k-1) * k^n * x^k )
 *
 * Each row n contains n+2 coefficients (for x^0 … x^(n+1)).
 * The sequence a(n) is obtained by listing the rows in order:
 *
 *   a(0)=t(0,0), a(1)=t(0,1), a(2)=t(1,0), a(3)=t(1,1), a(4)=t(1,2),
 *   a(5)=t(2,0), …
 *
 * The coefficient t(r,c) (0≤c≤r+1) is computed from the closed form:
 *
 *     t(r,c) = C(r+1,c)*(-2)^c + Σ_{k=1}^{c}
 *              ( 2^(k-1) * k^r * C(r+1,c-k) * (-2)^(c-k) )
 *
 * For r=0,1,… the row lengths are r+2 and the cumulative count up to
 * row r equals ((r+1)*(r+4))/2.  The row r containing index n satisfies
 * ((r+1)*(r+4))/2 > n ≥ (r*(r+3))/2.
 *
 * The code below uses only integer arithmetic and the helper functions
 * supplied by the evaluation framework.
 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/pow_int.h"
#include "../utils/binom.h"

static seq_t t_row_col(idx_t r, idx_t c)
{
    /* coefficient of x^c in p(x,r)   (0 ≤ c ≤ r+1) */
    seq_t res = 0;

    /* part from (1-2x)^(r+1) * 1  :  C(r+1,c) * (-2)^c  */
    seq_t bin_a   = binom(r + 1, c);
    seq_t pow2_c  = pow2(c);
    seq_t sign_a  = (c & 1) ? -((seq_t)1) : ((seq_t)1);
    res += bin_a * (sign_a * pow2_c);

    /* convolution with Σ_{k≥1} 2^(k-1) k^r x^k   */
    for (idx_t k = 1; k <= c; ++k) {
        seq_t coef_pow2   = pow2(k - 1);          /* 2^(k-1)       */
        seq_t k_pow_r     = pow_int((seq_t)k, r); /* k^r            */
        seq_t bin          = binom(r + 1, c - k); /* C(r+1,c-k)     */
        idx_t exp          = c - k;               /* exponent for (-2) */
        seq_t pow2_exp     = pow2(exp);           /* 2^(c-k)        */
        seq_t sign2        = (exp & 1) ? -((seq_t)1) : ((seq_t)1);
        seq_t term = coef_pow2 * k_pow_r * bin * (sign2 * pow2_exp);
        res += term;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine the row containing index n */
    idx_t r = 0;
    /* cumulative count up to row r: ((r+1)*(r+4))/2 */
    while ( ((r + 1) * (r + 4)) / 2 <= n )
        ++r;

    /* cumulative count up to previous row */
    idx_t prev_cum = (r == 0) ? 0 : (r * (r + 3)) / 2;

    /* column index within the row */
    idx_t c = n - prev_cum;          /* 0 ≤ c ≤ r+1 */

    return t_row_col(r, c);
}
