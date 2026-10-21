/*
 * A094417  Generalized ordered Bell numbers Bo(4,n).
 * https://oeis.org/A094417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Σ_{k=0}^{n-1} binom(n,k) * a(k), with a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine builds the sequence lazily and caches all computed values. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: array of already computed a(k) values. */
    static seq_t *cache = NULL;       /* pointer to cache memory      */
    static size_t cache_cap = 0;      /* allocated capacity           */
    static size_t cache_len = 0;      /* number of terms stored       */

    /* Initialise the cache with a(0)=1 when first called. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out‑of‑memory guard */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If the requested term is already in cache, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache if necessary to hold up to n (inclusive). */
    if (cache_len <= (size_t)n) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out‑of‑memory guard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;              /* binom(i,0) */

        /* k = 0 contributes cache[0] * 1 */
        sum += cache[0] * binom;

        /* compute C(i,k) iteratively for k = 1 .. i-1 */
        for (size_t k = 1; k < i; ++k) {
            /* binom(i,k) = binom(i,k-1) * (i-k+1) / k  */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            sum += cache[k] * binom;
        }

        /* a(i) = 4 * sum */
        cache[i] = sum * (seq_t)4;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
