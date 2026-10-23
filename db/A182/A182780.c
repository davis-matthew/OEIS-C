/*
 * A182780  Twice A024537.
 * https://oeis.org/A182780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3); Initial values: a(0)=2,
 *            a(1)=4, a(2)=8;
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>       /* for malloc, realloc, calloc */
#include <stdint.h>       /* for uint64_t used in idx_t */

/* Sequence entry function */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize cache with first three terms if not yet done */
    if (cache == NULL) {
        cache = (seq_t *)calloc(3, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache_size = 3;
    }

    /* Grow the cache up to the requested index n */
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        /* Compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 3*a(i-1) - a(i-2) - a(i-3) */
            cache[i] = 3 * cache[i - 1] - cache[i - 2] - cache[i - 3];
        }
        cache_size = new_cap;
    }

    return cache[n];
}
