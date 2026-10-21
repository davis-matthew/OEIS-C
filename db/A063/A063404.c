/*
 * A063404  a(0)=1; a(1)=1; a(2)=1; a(n) = a(n-1) + (1 + a(n-2))*(1 + a(n-3)).
 * https://oeis.org/A063404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(n)=a(n-1)+(1+a(n-2))*(1+a(n-3)) for
 *            n≥3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 11, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t *cache = NULL;        /* allocated array of seq_t */
    static size_t cache_cap = 0;       /* allocated capacity (bytes of seq_t) */
    static idx_t   cache_len = 0;      /* number of terms stored (cache_len ≤ cache_cap) */

    /* Allocate initial cache if first call */
    if (!cache) {
        cache_cap = 16;                /* initial capacity (in seq_t elements) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initialise the first three terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_len = 3;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(n) = a(n-1)+(1+a(n-2))*(1+a(n-3)) */
        seq_t term = cache[i-1]
                   + ((seq_t)1 + cache[i-2]) * ((seq_t)1 + cache[i-3]);
        cache[i] = term;
    }
    /* Update cache length if we extended it */
    if ((idx_t)(cache_len) <= n) {
        cache_len = n + 1;
    }

    return cache[n];
}
