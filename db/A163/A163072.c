/*
 * A163072  a(n) = ((4+sqrt(5))*(5+sqrt(5))^n + (4-sqrt(5))*(5-sqrt(5))^n)/2.
 * https://oeis.org/A163072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 20*a(n-2) ( n > 1 ), a(0) = 4, a(1) = 25
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoised cache of computed terms */
static seq_t *cache = NULL;     /* array of terms, dynamically resized */
static size_t cache_cap = 0;    /* current capacity of cache */

/* Return the nth term of OEIS A163072 */
seq_t a(idx_t n)
{
    /* initialise base cases if this is the first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();         /* unrecoverable allocation failure */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)25;
    }

    /* expand the cache up to index n, if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();     /* unrecoverable allocation failure */
        cache = new_cache;

        /* compute new entries using the recurrence */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            /* cache[i-1] and cache[i-2] are already defined */
            cache[(size_t)i] = (seq_t)10 * cache[(size_t)(i - 1)]
                               - (seq_t)20 * cache[(size_t)(i - 2)];
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
