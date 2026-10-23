/*
 * A117450  Expansion of (1-x+x^2+x^5)/((1-x)^2*(1-x^5)).
 * https://oeis.org/A117450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7), a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=7, a(6)=9
 * Range:     u8 n <= 35, u16 n <= 571, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   deallocate_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/deallocate_cache.h"

static seq_t *allocate_cache(idx_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t *)malloc((size_t)len * sizeof(seq_t));
#endif
}

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Hard‑coded initial values */
static const seq_t init_vals[7] = { 1, 1, 2, 3, 4, 7, 9 };

/* Ensure that cache holds values up to at least index n */
static void _ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        /* First allocation, store the seven initial terms */
        cache_len = 7;
        cache = allocate_cache(cache_len);
        for (idx_t i = 0; i < 7; ++i) {
            cache[i] = init_vals[i];
        }
    }

    if (n + 1 <= cache_len) /* already sufficient */
        return;

    idx_t old_len = cache_len;
    idx_t new_len = n + 1;

    seq_t *new_cache = allocate_cache(new_len);
    /* Copy old terms */
    for (idx_t i = 0; i < old_len; ++i) {
        new_cache[i] = cache[i];
    }
    /* Any missing initial terms (if old_len < 7) */
    if (old_len < 7) {
        for (idx_t i = old_len; i < 7 && i < new_len; ++i) {
            new_cache[i] = init_vals[i];
        }
    }
    /* Release old buffer */
    deallocate_cache(cache);
    cache = new_cache;
    cache_len = new_len;

    /* Compute new terms using the recurrence */
    idx_t start = old_len;
    if (start < 7) start = 7;   /* skip positions already set */
    for (idx_t i = start; i <= n; ++i) {
        seq_t t = 2 * cache[i - 1];
        t -= cache[i - 2];
        t += cache[i - 5];
        t -= 2 * cache[i - 6];
        t += cache[i - 7];
        cache[i] = t;
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n < 7)                /* handle the first seven terms directly */
        return init_vals[n];
    _ensure_cache(n);
    return cache[n];
}
