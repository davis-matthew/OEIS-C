/*
 * A218313  Hilltop maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any king-move
 *          neighbor in a random 0..2 n X 2 array.
 * https://oeis.org/A218313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 3*a(n-3) + 3*a(n-4) + 3*a(n-5) with
 *            a(1)=3, a(2)=15, a(3)=63, a(4)=249, a(5)=993 (Empirical
 *            recurrence, proven for the sequence.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but guard against accidental 0 */

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of elements stored */
    static size_t cache_cap = 0;  /* capacity of the allocated array */

    /* initial 5 terms */
    const seq_t init[5] = { (seq_t)3, (seq_t)15, (seq_t)63,
                            (seq_t)249, (seq_t)993 };

    /* if we have not yet initialized the cache, do it now */
    if (cache_cap == 0) {
        cache_cap = 16;                         /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        /* store the first 5 terms explicitly */
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    }

    /* if cache is large enough, return value */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* realloc failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        /* add the five previous terms */
        sum += cache[i - 1];
        sum += cache[i - 2];
        sum += cache[i - 3];
        sum += cache[i - 4];
        sum += cache[i - 5];
        cache[i] = sum * (seq_t)3;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
