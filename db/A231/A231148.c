/*
 * A231148  Array of coefficients of numerator polynomials of the rational
 *          function p(n, x - 1/x), where p(n,x) = (x^n - 1)/(x - 1).
 * https://oeis.org/A231148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 70, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the coefficient of x^{k-1} in the numerator polynomial
 * P_m(x) of the rational function p(m, x-1/x), where
 *     p(m, t) = (t^m - 1)/(t - 1),
 * and m = ceil(sqrt(n)).  The polynomial P_m(x) has degree 2(m–1) and
 * satisfies
 *     P_m(x) = x·P_{m-1}(x) + sum_{j=0}^{m-1} binom(m-1, j)(-1)^{m-1-j} x^{2j},
 * with P_1(x) = 1.  The terms of the sequence are read row‑wise:
 * a(1) = 1; a(2..4) are the coefficients of P_2(x); a(5..9) of P_3(x); etc.
 */

#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                   /* offset is 1; behaviour below offset is irrelevant */

    /* Determine the row m such that m^2 >= n. */
    double dn = sqrt((double)n);
    idx_t m = (idx_t)dn;
    if (m * m < n) ++m;

    /* Position of n within row m: 1 ≤ j ≤ 2m‑1. */
    idx_t prev_sq = (m - 1) * (m - 1);
    idx_t j = n - prev_sq;                 /* 1‑based */
    idx_t e = j - 1;                       /* exponent of x in P_m(x) (0‑based) */

    /* t = (m-1) – e.  It may be negative; keep it as signed. */
    long long t = (long long)(m - 1) - (long long)e;
    idx_t k_min = (t >= 0) ? (idx_t)t : (idx_t)(-t);   /* |t| */

    seq_t sum = 0;
    for (idx_t k = k_min; k <= m - 1; k += 2) {
        /* i = (k + t)/2.  k and t have the same parity, so numerator is even. */
        long long num = (long long)k + t;      /* k + t ≥ 0 because k ≥ |t| */
        idx_t i = (idx_t)(num / 2);

        seq_t c = binom(k, i);
        if (i & 1) c = -c;                    /* multiply by (-1)^i */
        sum += c;
    }
    return sum;
}
