/*
 * A172399  Expansion of x^7/(1-2*x-2*x^2+x^3+x^8).
 * https://oeis.org/A172399
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) satisfies * a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) - a(n-8)
 *            for n >= 8 * a(7) = 1 * a(n) = 0 for n < 7 * This recurrence
 *            follows from the generating function * x^7/(1-2*x-2*x^2+x^3+x^8)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n-th term of A172399.  The function is memoised so that
 * any order of calls up to any n ≥ 7 is handled efficiently.  For n < 7
 * the sequence is defined to be 0. */
seq_t a(idx_t n)
{
    /* Terms below the offset are zero, we don't need a cache for them. */
    if (n < 7)
        return 0;

    /* Static cache structure */
    static seq_t *cache = NULL;          /* array of computed terms */
    static size_t cache_cap = 0;          /* allocated array size */
    static idx_t   cache_max  = 0;        /* highest index computed */

    /* Ensure the cache is large enough */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);                   /* unrecoverable failure */
        /* initialise with zeros */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first 8 terms if this is the first run */
    if (cache_max < 7) {
        cache[0] = 0; cache[1] = 0; cache[2] = 0; cache[3] = 0;
        cache[4] = 0; cache[5] = 0; cache[6] = 0;
        cache[7] = 1;
        cache_max = 7;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 7) {
            val = 1;               /* already initialised, but keep for clarity */
        } else {
            /* 2*a(i-1) + 2*a(i-2) - a(i-3) - a(i-8) */
            val = cache[i-1];
            val = val + cache[i-1];      /* + a(i-1) again       */
            val = val + cache[i-2];
            val = val + cache[i-2];      /* + a(i-2) again       */
            val = val - cache[i-3];
            if (i >= 8)
                val = val - cache[i-8];
        }
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
