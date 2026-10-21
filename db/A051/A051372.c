/*
 * A051372  a(n+1) = a(n) + sum of digits of a(n)^2, with a(1) = 567.
 * https://oeis.org/A051372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(1) = 567, a(n+1) = a(n) + digitSum(a(n)^2) (n ≥ 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */
#include "../utils/digit_sum_internal.h"

/* Sequence a(n) defined by OEIS A051372 */
seq_t a(idx_t n)
{
    /* cache[0] holds a(1), cache[1] holds a(2), ... */
    static seq_t *cache     = NULL;  /* pointer to the allocated array          */
    static idx_t cache_len  = 0;      /* number of terms that have been computed */
    static idx_t cache_cap  = 0;      /* current capacity of the allocated array */

    if (n == 0)                 /* sequence is defined only for n ≥ 1          */
        return (seq_t)0;

    /* Initialise the first term if the cache is empty */
    if (cache_len == 0) {
        /* Ensure a non‑zero capacity */
        cache_cap  = 1;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache[0]   = (seq_t)567;
        cache_len  = 1;
    }

    /* Grow the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;          /* allocation failure: return safe value */
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute any missing terms up to index n */
    if (n > cache_len) {
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t prev = cache[i - 1];        /* a(i) is cache[i-1] */
            seq_t sq   = prev * prev;
            seq_t ds   = digit_sum_internal(sq);
            cache[i]   = prev + ds;           /* a(i+1) */
        }
        cache_len = n;
    }

    return cache[n - 1];   /* cache[0] is a(1) */
}
