/*
 * A131963  Expansion of f(x, x^2) * f(x^4, x^12) in powers of x where f(, )
 *          is Ramanujan's general theta function.
 * https://oeis.org/A131963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = coefficient of x^n in f(x, x^2) * f(x^4, x^12), * where
 *            f(a,b) = Σ_{k∈ℤ} a^{k(k+1)/2} b^{k(k-1)/2}. * The exponents of
 *            the first series are e₁(k)=k(3k-1)/2 for all * integer k. The
 *            exponents of the second series are e₂(k)=4k(2k-1) * for all
 *            integer k. a(n) is the number of ordered pairs...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 990 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ---------- helper to build a dynamic integer array ---------- */
static void push_exponent(idx_t **arr, size_t *len, size_t *cap, idx_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        idx_t *tmp = (idx_t *)realloc(*arr, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* ---------- compute a single value (without caching) ---------- */
static seq_t compute_a_single(idx_t target)
{
    idx_t *e1 = NULL; size_t sz1 = 0, cap1 = 0;
    idx_t *e2 = NULL; size_t sz2 = 0, cap2 = 0;

    idx_t val, k;

    /* generate e1 for k >= 0 */
    for (k = 0;; ++k) {
        val = k * (3 * k - 1) / 2;
        if (val > target) break;
        push_exponent(&e1, &sz1, &cap1, val);
    }
    /* generate e1 for k < 0  (k = -m, m>0) */
    for (k = 1;; ++k) {
        val = k * (3 * k + 1) / 2;
        if (val > target) break;
        push_exponent(&e1, &sz1, &cap1, val);
    }

    /* generate e2 for k >= 0 */
    for (k = 0;; ++k) {
        val = 4 * k * (2 * k - 1);
        if (val > target) break;
        push_exponent(&e2, &sz2, &cap2, val);
    }
    /* generate e2 for k < 0  (k = -m, m>0) */
    for (k = 1;; ++k) {
        val = 4 * k * (2 * k + 1);
        if (val > target) break;
        push_exponent(&e2, &sz2, &cap2, val);
    }

    seq_t coeff = 0;
    for (size_t i = 0; i < sz1; ++i) {
        idx_t a1 = e1[i];
        for (size_t j = 0; j < sz2; ++j) {
            if (a1 + e2[j] == target)
                ++coeff;
        }
    }

    free(e1);
    free(e2);
    return coeff;
}

/* ---------- main sequence function with memoisation ---------- */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n < cache_size) {
        return cache[n];
    }

    /* extend cache to include indices up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_size);
    if (!tmp) abort();
    cache = tmp;

    for (size_t i = cache_size; i < new_size; ++i) {
        cache[i] = compute_a_single((idx_t)i);
    }
    cache_size = new_size;
    return cache[n];
}
