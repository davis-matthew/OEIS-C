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
 * Validated: standard, pass, 57 terms through n = 934 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

static void compute_mu_up_to(idx_t up_to, int **mu_out, size_t *mu_cap_out)
{
    size_t n = (size_t)up_to;
    if (*mu_cap_out > n)
        return;                     /* already have enough */

    /* Allocate new μ-array */
    int *new_mu = (int *)malloc((n + 1) * sizeof(int));
    if (!new_mu) {
        /* out of memory – abort */
        exit(EXIT_FAILURE);
    }

    /* Linear sieve for μ */
    int *is_comp = (int *)calloc(n + 1, sizeof(int));
    size_t *primes = (size_t *)malloc((n + 1) * sizeof(size_t));
    if (!is_comp || !primes) {
        exit(EXIT_FAILURE);
    }
    size_t primes_cnt = 0;

    new_mu[0] = 0;
    new_mu[1] = 1;

    for (size_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = primes[j];
            size_t v = i * p;
            if (v > n) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                new_mu[v] = 0;
                break;
            } else {
                new_mu[v] = -new_mu[i];
            }
        }
    }

    free(is_comp);
    free(primes);

    free(*mu_out);        /* replace old array */
    *mu_out = new_mu;
    *mu_cap_out = n + 1;
}

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
            mem_copy(new_cache, cache, cache_cap * sizeof(seq_t));
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
