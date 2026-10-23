/*
 * A184884  Diagonal sums of number triangle A184883.
 * https://oeis.org/A184884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) + a(n-5) with
 *            initial values a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=11
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms stored in the cache after first call */
    static seq_t *cache = NULL;       /* array of computed terms */
    static size_t cache_size = 0;     /* number of terms stored */

    /* Allocate and initialise the cache if this is the first call. */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed: cannot compute sequence */
            return (seq_t)0;
        }
        /* a(0) ... a(4) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)11;
    }

    /* If the requested n is already known, return it immediately */
    if (n < cache_size) {
        return cache[n];
    }

    /* Extend the cache to hold terms up to n, computing new terms by recurrence */
    size_t new_size = (size_t)n + 1;            /* size_t is unsigned */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) { /* realloc failed */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute terms from cache_size up to n inclusive */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* Recurrence: a(i) = a(i-1) + 2*a(i-2) + 2*a(i-3) - a(i-4) + a(i-5) */
        seq_t term = cache[i - 1]
                   + (seq_t)2 * cache[i - 2]
                   + (seq_t)2 * cache[i - 3]
                   + cache[i - 5];
        term = term - cache[i - 4];
        cache[i] = term;
    }

    cache_size = new_size;  /* update the stored size */
    return cache[(size_t)n];
}
