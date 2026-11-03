/*
 * A144558  Expansion of Product_{n >= 1}
 *          (1+q^(2*n-1))/((1-q^(4*n))*(1+q^(4*n-2))).
 * https://oeis.org/A144558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in * ∏_{i≥1} (1+q^{2i-1}) /
 *            ((1-q^{4i})(1+q^{4i-2})) * * The sequence is computed by
 *            expanding the product as a * generating function. Coefficients
 *            are stored in a static * cache and recomputed when a larger
 *            index is requested. * * This algorithm uses dynamic
 *            programming...
 * Range:     i8 n <= 23, i16 n <= 69, i32 n <= 215, i64 n <= 717,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 946 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* ===== helper: compute all coefficients up to N ===== */
static void compute_seq(seq_t *coeff, idx_t N)
{
    size_t size = (size_t)N + 1;
    for (size_t i = 0; i < size; ++i) coeff[i] = 0;
    coeff[0] = 1;

    /* numerator: product (1+q^j) for odd j (0‑1 knapsack) */
    for (idx_t j = 1; j <= N; j += 2) {
        size_t sj = (size_t)j;
        size_t e = size - 1;
        while (e >= sj) {
            coeff[e] += coeff[e - sj];
            if (e == sj) break;
            --e;
        }
    }

    /* denominator part: product 1/(1-q^k) with k = 4*m (unbounded) */
    for (idx_t m = 1; (size_t)(4 * m) <= (size_t)N; ++m) {
        size_t k = 4 * m;
        for (size_t e = k; e < size; ++e) {
            coeff[e] += coeff[e - k];
        }
    }

    /* denominator part: product 1/(1+q^m) with m = 4*m-2 (alternating) */
    for (idx_t m = 1; (size_t)(4 * m - 2) <= (size_t)N; ++m) {
        size_t k = 4 * m - 2;
        for (size_t e = k; e < size; ++e) {
            coeff[e] -= coeff[e - k];
        }
    }
}

/* ===== main sequence function ===== */
static seq_t *cache = NULL;        /* cached coefficients */
static idx_t cached_size = 0;      /* number of cached coefficients */

seq_t a(idx_t n)
{
    /* Precompute up to at least 10000 on first call */
    if (cache == NULL) {
        idx_t target = (n > 10000) ? n : 10000;
        cache = mem_new(seq_t, (size_t)(target + 1));
        if (!cache) return (seq_t)0;   /* safety */
        compute_seq(cache, target);
        cached_size = target + 1;
    } else if (n >= cached_size) {
        /* Need more coefficients: recompute up to n */
        idx_t target = n;
        seq_t *new_cache = mem_new(seq_t, (size_t)(target + 1));
        if (!new_cache) return (seq_t)0;   /* safety */
        compute_seq(new_cache, target);
        mem_free(cache);
        cache = new_cache;
        cached_size = target + 1;
    }

    return cache[n];
}
