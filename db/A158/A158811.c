/*
 * A158811  Numerator of Hermite(n, 1/3).
 * https://oeis.org/A158811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 2*a(n-1) - 18*(n-1)*a(n-2) with a(0) = 1, a(1) = 2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Local static cache: values for indices 0 .. cache_len-1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Allocate the initial two values if the cache is empty */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* out‐of‑memory guard */
        cache[0] = 1;
        cache[1] = 2;
    }

    /* Extend cache up to n if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        /* Reallocate with the larger size */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache) return 0;      /* out‑of‑memory guard */
        /* Keep the original pointer for the case realloc fails */
        cache = new_cache;
        /* Compute new entries iteratively using the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t term1 = cache[i - 1];
            seq_t term2 = cache[i - 2];
            idx_t multiplier = i - 1;
            cache[i] = (seq_t)2 * term1 - (seq_t)18 * (seq_t)multiplier * term2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
