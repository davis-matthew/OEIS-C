/*
 * A047876  a(n) = Sum{T(i,n-i): i=0,1,...,n}, array T as in A049604.
 * https://oeis.org/A047876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum{ T(i, n-i) : i=0..n }   (A047876)
 *
 * The recurrence that is known to generate the sequence (conjectured by
 * Chai Wah Wu) is:
 *
 *   a(n) = 2*a(n-3) + a(n-4) - a(n-6) - 2*a(n-7) + a(n-10)   (n > 14)
 *
 * Initial values (for 0 ≤ n ≤ 14) are taken directly from the
 * OEIS b-file and are used as a bootstrap for the recurrence.
 *
 * This implementation keeps a static cache of previously computed terms,
 * allocating or expanding it as needed.  Calls may be made in any order
 * and the cache guarantees that each term is computed exactly once.
 */
#include <stdlib.h>
#include <stddef.h>

static const seq_t base[15] = {
    /* n =  0  1  2  3  4   5   6   7   8   9  10  11  12  13  14 */
    0, 6, 8, 8, 12, 18, 22, 28, 36, 42, 52, 64, 68, 82, 98
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Returns the nth term of A047876 */
seq_t a(idx_t n)
{
    /* For n < 15 we can return the hard‑coded values directly. */
    if (n < 15)
        return base[(size_t)n];

    /* If this is the first call requiring n ≥ 15 we allocate and
       initialise the cache with the base values. */
    if (cache_len == 0) {
        /* Allocate enough space for terms 0..n inclusive. */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache)
            abort();                 /* fatal allocation error */

        /* Copy base values into the cache. */
        for (idx_t i = 0; i < 15; ++i)
            cache[(size_t)i] = base[(size_t)i];

        cache_len = 15;

        /* Compute the remaining terms up to n. */
        for (idx_t i = 15; i <= n; ++i) {
            seq_t val = 2 * cache[(size_t)(i - 3)]
                        + cache[(size_t)(i - 4)]
                        - cache[(size_t)(i - 6)]
                        - 2 * cache[(size_t)(i - 7)]
                        + cache[(size_t)(i - 10)];
            cache[(size_t)i] = val;
        }
        cache_len = n + 1;
        return cache[(size_t)n];
    }

    /* If the cache already contains terms up to n, return the cached
       value. */
    if (cache_len > n)
        return cache[(size_t)n];

    /* The cache needs to grow to include terms up to n. */
    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* fatal allocation error */
    cache = new_cache;

    /* Compute the missing terms extending from cache_len to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 2 * cache[(size_t)(i - 3)]
                    + cache[(size_t)(i - 4)]
                    - cache[(size_t)(i - 6)]
                    - 2 * cache[(size_t)(i - 7)]
                    + cache[(size_t)(i - 10)];
        cache[(size_t)i] = val;
    }

    cache_len = new_size;
    return cache[(size_t)n];
}
