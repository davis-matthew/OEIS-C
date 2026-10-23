/*
 * A122558  a(0)=1, a(1)=3, a(n) = 4*a(n-1) + 3*a(n-2) for n > 1.
 * https://oeis.org/A122558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2); a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ANSI C function for the OEIS sequence A122558 */
seq_t a(idx_t n)
{
    /* static cache ensures that repeated calls are fast and order-agnostic */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;   /* number of computed terms */

    /* helper to grow the cache to accommodate the requested index */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* initialize the first two terms once */
    if (cache_len == 0) {
        if (cache_cap < 2) {
            /* should never happen due to the grow above */
            size_t init_cap = 2;
            cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
            cache_cap = (idx_t)init_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* return immediately if the value is already computed */
    if (n < cache_len)
        return cache[n];

    /* compute terms from cache_len up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 4 * cache[i-1] + 3 * cache[i-2];
    }
    cache_len = n + 1;

    return cache[n];
}
