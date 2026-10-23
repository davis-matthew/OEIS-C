/*
 * A131269  a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) with n>3, a(0)=1,
 *          a(1)=2, a(2)=3, a(3)=6.
 * https://oeis.org/A131269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-1) + n - 2, a(0)=1, a(1)=2, a(2)=3, a(3)=6
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Lazy initialization of the cache with the first four terms. */
    if (cache == NULL) {
        /* Start with a reasonable capacity. */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache_len = 4;
    }

    /* If we already have the requested term, just return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Ensure the cache is large enough. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;  /* double until sufficient */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;  /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-1) + a(i-2) + (i-2). */
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)(i - 2);
    }

    cache_len = n + 1;
    return cache[n];
}
