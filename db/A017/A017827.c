/*
 * A017827  a(n) = a(n-4) + a(n-5), with a(0)=1, a(1)=a(2)=a(3)=0, a(4)=1.
 * https://oeis.org/A017827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5), a(0) = 1, a(1)=a(2)=a(3)=0, a(4)=1
 * Range:     u8 n <= 44, u16 n <= 81, u32 n <= 153, u64 n <= 296,
 *            u128 n <= 583, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by recurrence with a static memoization cache.  */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache   = NULL;   /* array of already computed a(i) */
    static size_t  len    = 0;      /* number of valid entries in cache (last index = len-1) */
    static size_t  cap    = 0;      /* allocated capacity of the array */

    /* very small n – ensure that the base terms are defined */
    if (n < 5) {
        if (len < 5) {
            /* Allocate/resize to hold at least the first five terms */
            size_t new_cap = 5;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
            /* initialise the base terms */
            cache[0] = (seq_t)1;
            cache[1] = cache[2] = cache[3] = (seq_t)0;
            cache[4] = (seq_t)1;
            len = 5;
        }
        return cache[n];
    }

    /* For n >= 5 we may need to grow or extend the cache. */
    /* Ensure enough allocated capacity. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* If we just allocated the array without base terms, initialise them. */
    if (len < 5) {
        cache[0] = (seq_t)1;
        cache[1] = cache[2] = cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
        len = 5;
    }

    /* Compute missing values up to n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 4] + cache[i - 5];
    }

    len = (size_t)n + 1;   /* now cache contains a[0..n] */

    return cache[n];
}
