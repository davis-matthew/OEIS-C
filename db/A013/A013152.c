/*
 * A013152  E.g.f.:
 *          log(tanh(x)+cos(x))=x-2/2!*x^2+3/3!*x^3-12/4!*x^4+65/5!*x^5...
 * https://oeis.org/A013152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n/n! in * log(tanh(x)+cos(x)) *
 *            Recurrence: * let t[n] = coefficient of x^n/n! in tanh(x)+cos(x)
 *            * let a[0] = 0 , a[1] = t[1] * for n ≥ 2 * a[n] = t[n] − (1/n) *
 *            Σ_{k=1}^{n−1} k * C(n,k) * a[k] * t[n−k] * * The binomial C(n,k)
 *            is an integer. * All computations are per...
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/*  Return the n‑th term of the sequence A013152.
 *  idx_t is an unsigned integer type (usually uint64_t).
 *  seq_t is a signed integer type capable of arbitrary precision. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* Allocate space for coefficients */
    seq_t *t      = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *t_tanh = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *a_arr  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *binom  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!t || !t_tanh || !a_arr || !binom) {
        free(t); free(t_tanh); free(a_arr); free(binom);
        return (seq_t)0;
    }

    /* Initial cosine coefficients: t_even = (-1)^(i/2), t_odd = 0   */
    for (idx_t i = 0; i <= n; ++i) {
        if ((i & 1) == 0) {               /* even */
            idx_t half = i >> 1;
            t[i] = ((half & 1) ? (seq_t)-1 : (seq_t)1);
        } else {                           /* odd */
            t[i] = (seq_t)0;
        }
    }

    /* ----------   Compute tanh coefficients t_tanh   ----------
     * t_tanh[0] = 0  (tanh has no constant term)
     * t_tanh[1] = 1  (tanh(x) = x + …)
     * For n ≥ 1:  t_tanh[n+1] = - Σ_{k=1}^{n-1} t_tanh[k] * t_tanh[n-k] * C(n,k)
     */
    t_tanh[0] = (seq_t)0;
    t_tanh[1] = (seq_t)1;
    for (idx_t m = 1; m <= n - 1; ++m) {
        /* Compute binomial coefficients C(m,k) for this row */
        binom[0] = (seq_t)1;
        for (idx_t k = 1; k <= m; ++k) {
            binom[k] = binom[k - 1] * (seq_t)(m - k + 1) / (seq_t)k;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m - 1; ++k) {
            seq_t term = t_tanh[k] * t_tanh[m - k];
            term *= binom[k];
            sum += term;
        }
        t_tanh[m + 1] = -sum;
    }

    /* Add tanh part to the full series t */
    for (idx_t i = 1; i <= n; ++i) t[i] += t_tanh[i];

    /* ----------  Compute a[n] coefficients  ----------
     * a[0] = 0
     * a[1] = t[1]
     * For n ≥ 2:  a[n] = t[n] - (1/n) * Σ_{k=1}^{n-1} k * C(n,k) * a[k] * t[n-k]
     */
    a_arr[0] = (seq_t)0;
    a_arr[1] = t[1];

    for (idx_t NN = 2; NN <= n; ++NN) {
        /* Binomial coefficients for this N */
        binom[0] = (seq_t)1;
        for (idx_t k = 1; k <= NN; ++k) {
            binom[k] = binom[k - 1] * (seq_t)(NN - k + 1) / (seq_t)k;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < NN; ++k) {
            seq_t term = a_arr[k] * t[NN - k];
            term *= (seq_t)k;
            term *= binom[k];
            sum += term;
        }
        seq_t div = sum / (seq_t)NN;          /* exact division */
        a_arr[NN] = t[NN] - div;
    }

    seq_t result = a_arr[n];

    free(t);
    free(t_tanh);
    free(a_arr);
    free(binom);
    return result;
}
