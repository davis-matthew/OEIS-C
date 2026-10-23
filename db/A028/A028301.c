/*
 * A028301  a(n) = a(n-1)^2 + prime(n-1), with a(1) = 1.
 * https://oeis.org/A028301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1)^2 + prime(n-1),  a(1) = 1 */

#include <stdlib.h>          /* malloc, free, size_t */
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t) */

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* pointer to allocated array */
static size_t cache_cap = 0;  /* current capacity of cache */
static size_t cache_len = 0;  /* number of terms actually stored */

/* Helper: extend cache to at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;                 /* already sufficient */

    seq_t *newp = (seq_t*)malloc(need * sizeof(seq_t));
    if (!newp) {
        /* allocation failure – abort (no exception handling in C) */
        abort();
    }

    /* copy old data, if any */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            newp[i] = cache[i];
        free(cache);
    }

    cache = newp;
    cache_cap = need;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – return 0 for safety */
        return (seq_t)0;

    /* initialise base term a(1) if cache empty */
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if ((size_t)n <= cache_len)      /* already computed */
        return cache[(size_t)n - 1];

    /* extend cache to contain terms up to n */
    ensure_cache((size_t)n);

    /* compute missing terms */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t prev = cache[(size_t)i - 2];                 /* a(i-1) */
        seq_t sq   = prev * prev;                          /* a(i-1)^2   */
        seq_t pr   = (seq_t)get_prime((idx_t)i - 1);       /* prime(i-1) */
        cache[(size_t)i - 1] = sq + pr;                    /* a(i)       */
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
