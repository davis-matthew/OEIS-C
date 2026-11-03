/*
 * A033792  Product t2(q^d); d | 33, where t2(q) = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|33} t(q^d), where t(q) =
 *            Σ_{k≥0} q^{k(k+1)}
 * Range:     u8 n <= 1523, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9729 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses convolution of power‑series whose exponents
   are the triangular numbers multiplied by the divisors of 33
   (1, 3, 11, 33). */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the coefficient array up to N (inclusive). */
static seq_t *compute_product(idx_t N)
{
    /* Allocate the zero array.  Use an explicit cast for C++ compatibility. */
    seq_t *coeff = mem_new(seq_t, (size_t)(N + 1));
    if (!coeff) return NULL;
    coeff[0] = 1;              /* t(0) = 1 */

    /* Small array of the divisors of 33. */
    const int divisors[] = {1, 3, 11, 33};
    const size_t num_div = sizeof(divisors) / sizeof(divisors[0]);

    for (size_t d_idx = 0; d_idx < num_div; ++d_idx)
    {
        int d = divisors[d_idx];

        /* First pass: count how many exponents fit in the range. */
        size_t exp_cnt = 0;
        for (idx_t k = 0; ; ++k)
        {
            idx_t tri = k * (k + 1) / 2;
            idx_t e   = (idx_t)d * tri;
            if (e > N) break;
            ++exp_cnt;
        }

        /* Allocate the exponent array. */
        idx_t *exps = (idx_t*)malloc(exp_cnt * sizeof(idx_t));
        if (!exps) {
            mem_free(coeff);
            return NULL;
        }

        /* Second pass: store the exponents. */
        size_t pos = 0;
        for (idx_t k = 0; ; ++k)
        {
            idx_t tri = k * (k + 1) / 2;
            idx_t e   = (idx_t)d * tri;
            if (e > N) break;
            exps[pos++] = e;
        }

        /* Convolution: multiply the current series by the new factor. */
        seq_t *new_coeff = mem_new(seq_t, (size_t)(N + 1));
        if (!new_coeff) {
            mem_free(coeff);
            free(exps);
            return NULL;
        }

        for (idx_t i = 0; i <= N; ++i)
        {
            seq_t ci = coeff[i];
            if (!ci) continue;       /* skip zero terms to save work */
            for (size_t j = 0; j < exp_cnt; ++j)
            {
                idx_t idx = i + exps[j];
                if (idx > N) break;   /* exps[j] are increasing */
                new_coeff[idx] += ci;
            }
        }

        mem_free(coeff);
        coeff = new_coeff;
        free(exps);
    }

    return coeff;
}

static seq_t *cache = NULL;   /* array of coefficients up to current cache_len-1 */
static idx_t cache_len = 0;   /* number of terms stored */

/* Extend the cache up to at least n. */
static void ensure_cache(idx_t n)
{
    if (cache && n < cache_len) return;   /* already have it */
    if (cache) mem_free(cache);
    cache = compute_product(n);
    if (cache) cache_len = n + 1;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache ? cache[n] : 0;
}
