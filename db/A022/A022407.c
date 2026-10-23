/*
 * A022407  a(n) = a(n-1) + a(n-2) + 1, with a(0)=3, a(1)=8.
 * https://oeis.org/A022407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, with a(0)=3, a(1)=8
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A022407.  The function is
   thread‑unsafe but works correctly for any order of calls.  It uses
   a static cache so that repeated calls are fast.  The implementation
   is valid C11 and C++17. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;   /* pointer to array cache[0 … cache_cap-1] */
    static idx_t cache_cap = 0;   /* capacity of the allocated array */
    static idx_t cache_size = 0;  /* number of terms that are actually
                                    computed and stored in cache */

    /* Base cases. */
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)8;

    /* Initialise the cache on the first call. */
    if (cache_size == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)  /* unlikely to happen in the tests */
            return (seq_t)0;
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)8;
        cache_size = 2;
    }

    /* Extend the cache (and compute missing terms) if needed. */
    if (n >= cache_size) {
        idx_t need = n + 1;

        /* Grow the allocated capacity if necessary. */
        if (need > cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)  /* unlikely to happen */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute the remaining terms using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)1;
        }
        cache_size = need;
    }

    return cache[n];
}
