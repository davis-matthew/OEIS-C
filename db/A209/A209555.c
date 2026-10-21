/*
 * A209555  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209556; see the Formula section.
 * https://oeis.org/A209555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^{k} in u(r,x) where * r = smallest
 *            integer with r(r+1)/2 ≥ n, * k = n - r(r-1)/2 - 1. * Computing
 *            u(r,x) coefficient: * a_{r,k} = C(r-1,k) + Σ_{t=k+1}^{r-2}
 *            C(t,k) (t-k odd) * with C(n,k) the binomial coefficient. * This
 *            implementation uses only integer arithmetic in seq_t
 * Range:     u8 n <= 59, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   binom_iter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/binom_iter.h"

/* main function required by the evaluation system */
seq_t a(idx_t n) {
    /* find the row r such that r(r+1)/2 >= n > r(r-1)/2 */
    idx_t r = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);
    /* correct potential rounding errors */
    while (( (__int128)r * (r + 1) ) / 2 < n) ++r;
    while (((__int128)(r - 1) * r) / 2 >= n) --r;

    idx_t start = (idx_t)(( (__int128)r * (r - 1) ) / 2) + 1;
    idx_t k = n - start;      /* column index in the row (0‑based) */
    idx_t m = r;              /* row index (also polynomial degree m-1) */

    /* leading coefficient (k = m-1) is always 1 */
    if (k == m - 1) return (seq_t)1;

    /* binomial values for t = k, then update for t = k+1 … m-2 */
    seq_t binom_curr = (seq_t)1;   /* C(k,k) = 1 */
    seq_t sum = 0;

    for (idx_t t = k + 1; t <= m - 2; ++t) {
        binom_curr = binom_curr * (seq_t)t / (seq_t)(t - k);
        if (((t - k) & 1) != 0)
            sum += binom_curr;
    }

    /* compute C(m-1, k) from the last binom_curr (now C(m-2,k)) */
    seq_t binom_final = binom_curr * (seq_t)(m - 1)
                      / (seq_t)(m - 1 - k);

    return binom_final + sum;
}
