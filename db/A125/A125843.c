/*
 * A125843  a(1)=1. a(n) = the sum of the terms from among the first
 *          floor(n/2) terms of the sequence which are coprime to n.
 * https://oeis.org/A125843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=sum_{k=1}^{⌊n/2⌋} a(k) such that gcd(a(k),n)=1. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Cached values: 1‑based array where cache[i] = a(i). */
static seq_t *cache       = NULL;   /* array of a(k) */
static size_t cache_cap   = 0;      /* allocated capacity (index+1) */
static idx_t  cache_computed = 0;   /* highest index already computed */

/* Ensure the cache holds values up to index n (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Start with capacity for indices 0 and 1. */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[1] = (seq_t)1;
        cache_computed = 1;
    }

    /* Grow the array if needed. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing a(i) up to n. */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        idx_t half = i >> 1;          /* floor(i/2) */
        seq_t sum = (seq_t)0;
        seq_t n_seq = (seq_t)i;
        for (idx_t k = 1; k <= half; ++k) {
            if (gcd(cache[k], n_seq) == (seq_t)1)
                sum += cache[k];
        }
        cache[i] = sum;
    }
    cache_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1 – return 0 as a safe value */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
