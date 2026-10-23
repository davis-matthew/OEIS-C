/*
 * A025775  Expansion of 1/((1-x)(1-x^4)(1-x^11)).
 * https://oeis.org/A025775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 4, and 11
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-4) - a(n-5) + a(n-11) - a(n-12) - a(n-15) + a(n-16) */
/* Initial value: a(0) = 1, with a(k) = 0 for any negative k. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static caching of computed values */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;           /* current allocation size (in elements) */
    static idx_t   computed = 0;        /* number of entries actually computed */

    /* Ensure cache is allocated */
    if (cache == NULL) {
        /* start with a small capacity, grow as needed */
        cap = 64;
        if (n + 1 > cap) {
            /* if requested n is larger than initial capacity, just jump to it */
            cap = n + 1;
        }
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failed – return 0 as a fallback */
        computed = 0;
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        /* double until we have enough space */
        while (cap < (size_t)n + 1)
            cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (new_cache == NULL)
            return 0; /* allocation failed – return 0 */
        memcpy(new_cache, cache, computed * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }

    /* Compute values up to n, if not already done */
    if (computed <= n) {
        for (idx_t i = computed; i <= n; ++i) {
            /* Base case */
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t pos = 0;
            seq_t neg = 0;

            if (i >= 1) pos += cache[i-1];
            if (i >= 4) pos += cache[i-4];
            if (i >= 11) pos += cache[i-11];
            if (i >= 16) pos += cache[i-16];

            if (i >= 5)  neg += cache[i-5];
            if (i >= 12) neg += cache[i-12];
            if (i >= 15) neg += cache[i-15];

            /* The recurrence guarantees pos >= neg, so unsigned subtraction is safe */
            cache[i] = pos - neg;
        }
        computed = n + 1;
    }

    return cache[n];
}
