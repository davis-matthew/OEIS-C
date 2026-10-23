/*
 * A146956  A functionally symmetric Polynomial as a triangle of coefficients:
 *          p(x,n)=If[n == 0, 1, (x + 1)^n + 2^(n - 2)*Sum[(2^(m-1) + 2*m-2
 *          )*x^m*(1 + x^(n - 2*m)), {m, 1, n - 1}]].
 * https://oeis.org/A146956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A146956
 *  Functionally symmetric Polynomial coefficients, flattened row-wise.
 *
 *  For n ≥ 0 let r = ⌊(√(8 n+1)−1)/2⌋ be the row index such that
 *  r(r+1)/2 ≤ n < (r+1)(r+2)/2 and let m = n − r(r+1)/2.
 *  Then a(n) = t(r,m) where t(r,m) is the coefficient of x^m in
 *
 *          p(x,r) = (x+1)^r
 *                + 2^(r−2) * Σ_{k=1}^{r−1} (2^(k−1)+2k−2) · x^k · (1 + x^(r−2k)).
 *
 *  The coefficient can be computed directly by
 *
 *          t(r,m) = C(r,m)
 *                 + 2^(r−2)·(2^(m−1)+2m−2)            (from k=m)
 *                 + 2^(r−2)·(2^(r−m−1)+2(r−m)−2)      (from k=r−m)
 *
 *  with the special case t(r,0)=t(r,r)=1.  The function below implements
 *  this formula using the utilities pow2 and binomial from the utils library.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r, m;

    /* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    double d = sqrt((double)8 * (double)n + 1.0);
    r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors of the floating‑point estimate. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    m = n - r * (r + 1) / 2;          /* position inside the row        */

    /* Edge rows: r = 0 or r = 1, and all the outer coefficients. */
    if (r < 2)           return (seq_t)1;
    if (m == 0 || m == r)  return (seq_t)1;

    /* Base binomial contribution. */
    seq_t result = binomial(r, m);

    /* Common factor 2^(r-2). */
    seq_t two_pow_r_minus2 = pow2(r - 2);

    /* Term from k = m.  Compute (2^(m-1)+2m-2) as a seq_t. */
    seq_t coeff_m = pow2(m - 1);
    coeff_m += (seq_t)(2 * m - 2);
    seq_t term_m = two_pow_r_minus2 * coeff_m;

    if (m == r - m) {
        /* Central term – the two contributions coincide. */
        result += term_m;
        result += term_m;
    } else {
        /* Term from k = r-m.  Compute (2^(r-m-1)+2(r-m)-2). */
        seq_t coeff_rm = pow2(r - m - 1);
        coeff_rm += (seq_t)(2 * (r - m) - 2);
        seq_t term_rm = two_pow_r_minus2 * coeff_rm;

        result += term_m;
        result += term_rm;
    }

    return result;
}
