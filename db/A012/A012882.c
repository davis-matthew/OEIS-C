/*
 * A012882  Expansion of e.g.f.:
 *          exp(sech(x)*arctanh(x))=1+x+1/2!*x^2-3/4!*x^4+20/5!*x^5+165/6!*x^6...
 * https://oeis.org/A012882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in exp(sech(x)*arctanh(x)), times n!
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"      /* seq_gcd is available */
#include "../utils/binomial.h" /* binomial(idx_t n, idx_t k) returns seq_t */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* ---------- basic integer arrays ---------- */
    size_t N = (size_t)n;
    seq_t *fact   = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *euler  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *pk     = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *A      = (seq_t *)malloc((N + 1) * sizeof(seq_t));

    if (!fact || !euler || !pk || !A) {
        /* allocation failed */
        free(fact); free(euler); free(pk); free(A);
        return 0;
    }

    /* --- factorials --- */
    fact[0] = 1;
    for (size_t i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* --- Euler numbers E_{2m} for m = 0..N/2 --- */
    for (size_t i = 0; i <= N; ++i) euler[i] = 0;
    euler[0] = 1;                     /* E0 = 1 */
    for (size_t m = 1; m <= N / 2; ++m) {
        seq_t sum = 0;
        for (size_t k = 0; k < m; ++k) {
            idx_t i = 2 * k;
            seq_t bin = binomial(2 * m, i);
            sum += bin * euler[i];
        }
        euler[2 * m] = -sum;
    }

    /* --- precompute k! * g_k = p_k (integer) --- */
    for (size_t i = 0; i <= N; ++i) pk[i] = 0;
    for (size_t k = 1; k <= N; ++k) {
        if ((k & 1) == 0) continue;            /* p_k is zero for even k   */
        seq_t sum = 0;
        for (idx_t i = 0; i <= (idx_t)k; i += 2) {   /* i even */
            idx_t j = k - i;                 /* j is odd and >0         */
            seq_t bin = binomial(k, i);
            seq_t fact_jm1 = fact[j - 1];
            seq_t term = bin * fact_jm1;
            term *= euler[i];
            sum += term;
        }
        pk[k] = sum;
    }

    /* --- compute A[n] = n! * a(n) via recurrence --- */
    A[0] = 1;
    for (size_t n_i = 1; n_i <= N; ++n_i) {
        seq_t sum = 0;
        seq_t C   = 1;  /* C = binomial(n_i-1, k-1) for current k */
        for (size_t k = 1; k <= n_i; ++k) {
            if (pk[k] != 0) {
                seq_t temp = pk[k] * A[n_i - k] * C;
                sum += temp;
            }
            if (k < n_i) {
                /* Update C to binomial(n_i-1, k) from binomial(n_i-1, k-1) */
                seq_t num = C * (seq_t)(n_i - k);
                seq_t den = (seq_t)k;
                C = num / den;  /* division exact */
            }
        }
        A[n_i] = sum;
    }

    seq_t result = A[n];
    /* free memory */
    free(fact);
    free(euler);
    free(pk);
    free(A);
    return result;
}
