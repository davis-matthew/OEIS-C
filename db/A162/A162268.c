/*
 * A162268  a(n) = ((5+sqrt(2))*(1+sqrt(2))^n + (5-sqrt(2))*(1-sqrt(2))^n)/2.
 * https://oeis.org/A162268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0) = 5, a(1) = 7
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) {
        if (cache != NULL && cache_len > 0) return cache[0];
        return (seq_t)5;
    }
    if (n == 1) {
        if (cache != NULL && cache_len > 1) return cache[1];
        return (seq_t)7;
    }

    /* If no cache yet, build up to n */
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)7;
        for (idx_t i = 2; i <= n; ++i) {
            /* a(i) = 2*a(i-1) + a(i-2) */
            cache[i] = (cache[i-1] + cache[i-1]) + cache[i-2];
        }
        return cache[n];
    }

    /* If we already have all values up to n, return immediately */
    if (cache_len > n) {
        return cache[n];
    }

    /* Need to expand the cache */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* allocation failed */
    cache = tmp;

    /* Ensure the first two entries are set */
    if (old_len == 0) {
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)7;
        old_len = 2;
    } else if (old_len < 2) {
        cache[1] = (seq_t)7;
        old_len = 2;
    }

    /* Compute new values from the point where we left off */
    for (idx_t i = old_len; i <= n; ++i) {
        cache[i] = (cache[i-1] + cache[i-1]) + cache[i-2];
    }
    return cache[n];
}
