/*
 * A154816  A triangular sequence of polynomial coefficients: p(x,n)=((-1)^(n
 *          + 1)*(x - 1)^(n + 1)*Sum[(5*m + 4)^n*x^m, {m, 0, Infinity}] +
 *          (-1)^(n + 1)*(x - 1)^(n + 1)*Sum[(5*m + 1)^n*x^m, {m, 0,
 *          Infinity}]).
 * https://oeis.org/A154816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = coefficient t(r,m) of the polynomial p(x,r) where r is
 *            the row such that r(r+1)/2 <= n < (r+1)(r+2)/2, and m = n -
 *            r(r+1)/2. The coefficient is given by: t(r,m) = Σ_{k=0}^{m}
 *            (-1)^k * C(r+1,k) * [ (5(m-k)+1)^r + (5(m-k)+4)^r ] where
 *            C(r+1,k) is the binomial coefficient
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* find row r */
    double discr = sqrt((double)1 + 8.0 * (double)n);
    idx_t r = (idx_t)((discr - 1.0) / 2.0);
    /* correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* column within the row */
    idx_t m = n - r * (r + 1) / 2;

    seq_t result = 0;
    seq_t binom   = 1; /* C(r+1, 0) */

    for (idx_t k = 0; k <= m; ++k) {
        idx_t s = m - k;                      /* m - k */
        uint64_t base1 = 5 * s + 1;            /* 5*(m-k)+1 */
        uint64_t base4 = 5 * s + 4;            /* 5*(m-k)+4 */

        seq_t pow1 = pow_uint(base1, r);
        seq_t pow4 = pow_uint(base4, r);
        seq_t term = pow1 + pow4;

        if (k & 1) {
            result -= term * binom;
        } else {
            result += term * binom;
        }

        if (k < m) {
            seq_t num = (seq_t)((r + 1) - k);
            binom = binom * num / (seq_t)(k + 1);
        }
    }
    return result;
}
