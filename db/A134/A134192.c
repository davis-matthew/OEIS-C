/*
 * A134192  a(0) = a(1) = 1. a(n) = Sum_{p|n} a(n-p), where the sum is over
 *          all distinct primes p that divide n.
 * https://oeis.org/A134192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=0 or 1; otherwise a(n) = Σ_{p|n} a(n-p) where p
 *            runs over distinct primes dividing n
 * Range:     u8 n <= 35, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper to build smallest‑prime‑factor table */
#include "../utils/build_spf.h"

/* Memoization cache for fixed indices */
static seq_t *cache = NULL;       /* cache[i] holds a(i); 0 means uncomputed */
static size_t cache_size = 0;

/* Smallest‑prime‑factor array (spf[i] = smallest prime factor of i) */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Recursive computation with memoisation */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 1)
        return 1;

    /* Grow cache to accommodate n (if needed) */
    if (cache == NULL || n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();       /* allocation failed */
        /* Initialise new entries to 0 */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
        /* Ensure base values are present */
        cache[0] = 1;
        if (cache_size > 1)
            cache[1] = 1;
    }

    /* Return memoised result if already computed */
    if (cache[n] != 0)
        return cache[n];

    /* Build spf table up to n if not yet present */
    if (n > spf_limit) {
        idx_t *new_spf = NULL;
        build_spf(n, &new_spf);
        free(spf);
        spf = new_spf;
        spf_limit = n;
    }

    /* Factor n via spf and sum contributions from distinct primes */
    idx_t temp = n;
    seq_t sum = 0;
    while (temp > 1) {
        idx_t p = spf[temp];
        sum += a(n - p);           /* only distinct primes are visited once */
        while (temp % p == 0)
            temp /= p;
    }

    /* Memoise and return */
    cache[n] = sum;
    return sum;
}
