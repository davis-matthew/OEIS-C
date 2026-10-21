/*
 * A215683  n-th derivative of sech(x)^tanh(x) at x=0.
 * https://oeis.org/A215683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n-th derivative at x = 0 of sech(x)^tanh(x) Computation
 *            uses the EGF recurrence f_n = sum_{k=1}^{n} C(n-1,k-1) g_k
 *            f_{n-k}, where g_k = - sum_{i=0}^{k-1} C(k,i) u_i u_{k-i-1} and
 *            u_0 = 0, u_1 = 1, u_{m+1} = - sum_{i=0}^{m} C(m,i) u_i u_{m-i}
 *            (m>=1) (u_m are tanh(x) EGF coefficients). All a...
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* Allocate arrays for u, g and a coefficients */
    seq_t *u = (seq_t *) malloc((n + 2) * sizeof(seq_t));  /* u[0..n+1] needed */
    seq_t *g = (seq_t *) malloc((n + 1) * sizeof(seq_t));  /* g[1..n] */
    seq_t *f = (seq_t *) malloc((n + 1) * sizeof(seq_t));  /* f[0..n] */

    /* initialise u */
    u[0] = 0;
    u[1] = 1;     /* tanh'(0) = 1 */

    /* compute u[2..n] using recurrence */
    idx_t m;
    for (m = 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t i;
        for (i = 0; i <= m; ++i) {
            seq_t term = binom(m, i);
            term = term * u[i] * u[m - i];
            sum = sum + term;
        }
        u[m + 1] = -sum;
    }

    /* compute g[1..n] */
    for (m = 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t k;
        for (k = 0; k <= m - 1; ++k) {
            seq_t term = binom(m, k);
            term = term * u[k] * u[m - k - 1];
            sum = sum + term;
        }
        g[m] = -sum;
    }

    /* compute f[0..n] */
    f[0] = 1; /* a(0) = 1 */
    for (m = 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t k;
        for (k = 1; k <= m; ++k) {
            seq_t term = binom(m - 1, k - 1);
            term = term * g[k] * f[m - k];
            sum = sum + term;
        }
        f[m] = sum;
    }

    seq_t result = f[n];

    /* free memory */
    free(u);
    free(g);
    free(f);

    return result;
}
