/*
 * A027405  Number of labeled servers of dimension 18.
 * https://oeis.org/A027405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of labeled servers of dimension 18 e.g.f.: exp(
 *            ∑_{i=1}^{18} ((1+x)^i-1)/i )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* cache for sequence values */
static seq_t *A = NULL;
static size_t A_cap = 0;
static idx_t A_len = 0;

/* precomputed coefficients g_k = (k-1)! * binom(18,k), 1 ≤ k ≤ 18 */
static seq_t g[19];
static bool coeffs_initialized = false;

/* ensure A has space for indices up to size-1 */
static void ensure_capacity(size_t size)
{
    if (A_cap < size) {
        size_t new_cap = size;
        if (new_cap < 64) new_cap = 64;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory: terminate */
        A = tmp;
        A_cap = new_cap;
    }
}

/* precompute g_k coefficients once */
static void init_coeffs(void)
{
    if (coeffs_initialized) return;
    coeffs_initialized = true;

    /* compute binom(18,k) for 0≤k≤18 */
    seq_t C[19];
    C[0] = 1;
    for (idx_t k = 1; k <= 18; ++k) {
        C[k] = binomial(18, k);
    }

    /* factorials 0! … 18! */
    seq_t fact[19];
    fact[0] = 1;
    for (idx_t i = 1; i <= 18; ++i) {
        fact[i] = fact[i - 1] * (idx_t)i;
    }

    /* g_k = (k-1)! * C[k]  (with g[0] unused) */
    for (idx_t k = 1; k <= 18; ++k) {
        g[k] = fact[k - 1] * C[k];
    }
}

/* main requested function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* base case */

    ensure_capacity((size_t)n + 1);
    init_coeffs();

    /* initialize first terms if not yet done */
    if (A_len == 0) {
        A[0] = 1;
        A_len = 1;
    }

    /* compute all missing values up to n */
    for (idx_t idx = A_len; idx <= n; ++idx) {
        seq_t sum = 0;
        idx_t maxk = idx < 18 ? idx : 18;
        seq_t binCoef = 1;            /* binom(idx-1,0) */

        for (idx_t k = 1; k <= maxk; ++k) {
            /* term = g_k * binomial(idx-1, k-1) * a[idx-k] */
            seq_t term = g[k];
            term *= binCoef;
            term *= A[idx - k];
            sum += term;

            /* prepare binCoef for next k: binom(idx-1,k) */
            if (k < maxk) {
                /* binCoef = binCoef * (idx - k) / k */
                binCoef *= (idx - k);
                binCoef /= k;
            }
        }
        A[idx] = sum;
    }
    A_len = n + 1;
    return A[(size_t)n];
}
