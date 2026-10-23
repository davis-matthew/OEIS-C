/*
 * A185694  Eigensequence for the Moebius mu triangle A152904.
 * https://oeis.org/A185694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(0) = 1 * For n ≥ 1: a(n) = -∑_{k=1..n} a(n-k) * μ(k) * where
 *            μ(k) is the Möbius function. * * This implementation caches the
 *            results so that any call order is supported. * It computes μ(k)
 *            once up to the largest n requested using a linear sieve * and
 *            keeps the sequence values in a dynamically...
 * Range:     i8 n <= 16, i16 n <= 27, i32 n <= 57, i64 n <= 116,
 *            i128 n <= 233, big any n
 * Helpers:   compute_mu_up_to, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/compute_mu_up_to.h"

/* ---------- local helpers for μ(k) ----------------------------------- */

/* ---------- main sequence function ----------------------------------- */

seq_t a(idx_t n)
{
    /* static cache of a(n) values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated length */
    static size_t cache_max = 0;   /* highest computed index +1 */

    /* static μ(k) array */
    static int *mu = NULL;
    static size_t mu_cap = 0;      /* allocated length */

    /* ensure cache is large enough */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;        /* double until enough */
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (!new_cache) exit(EXIT_FAILURE);
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            seq_free_array(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute μ up to n if needed */
    if (mu_cap <= (size_t)n + 1) {
        compute_mu_up_to((idx_t)n, &mu, &mu_cap);
    }

    /* initial value a(0) */
    if (cache_max == 0) {
        cache[0] = (seq_t)1;
        cache_max = 1;
    }

    /* compute a values up to n */
    for (size_t i = cache_max; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= i; ++k) {
            sum += cache[i - k] * (seq_t)mu[k];
        }
        cache[i] = -sum;
    }
    cache_max = (size_t)n + 1;

    return cache[(size_t)n];
}
