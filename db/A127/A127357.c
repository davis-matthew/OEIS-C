/*
 * A127357  Expansion of 1/(1 - 2*x + 9*x^2).
 * https://oeis.org/A127357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 9*a(n-2) with a(0)=1, a(1)=2
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 40, i128 n <= 80,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static idx_t   cache_len = 0;

/* Return a[n] for any n ≥ 0, building the cache on demand. */
seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Lazy allocation / expansion of the cache */
    if (cache == NULL) {
        /* initial capacity – enough for the first few terms */
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((idx_t)n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 1] - (seq_t)9 * cache[i - 2];
        }
        cache_len = (idx_t)n + 1;
    }

    return cache[n];
}
