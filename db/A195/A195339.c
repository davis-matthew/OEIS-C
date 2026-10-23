/*
 * A195339  Expansion of 1/(1-4*x+2*x^3+x^4).
 * https://oeis.org/A195339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-3) - a(n-4), with initial values a(0)=1,
 *            a(1)=4, a(2)=16, a(3)=62
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* small base values – hard‑coding a few terms is allowed */
    static const seq_t base[4] = { (seq_t)1, (seq_t)4, (seq_t)16, (seq_t)62 };

    /* memoisation cache for larger indices */
    static seq_t *cache = NULL;      /* array holding computed terms */
    static size_t cache_len = 0;     /* number of valid entries in cache */
    static size_t cache_cap = 0;     /* current capacity of the allocated array */

    /* trivial cases – no allocation needed */
    if (n < 4)
        return base[(size_t)n];

    /* initialise cache on first call to hold the first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache_len  = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();         /* cannot continue without memory */
        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
    }

    /* ensure the array can hold the requested index */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (idx >= new_cap) {
            new_cap <<= 1;             /* double until large enough */
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms up to idx, if necessary */
    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t val = (seq_t)4 * cache[i-1]
                        - (seq_t)2 * cache[i-3]
                        -      cache[i-4];
            cache[i] = val;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
