/*
 * A080878  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0)=1, a(1)=1, a(2)=3.
 * https://oeis.org/A080878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) - 4*a(n-4) (n ≥ 4) Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=4 Recurrence order 4, linear homogeneous.
 *            The function memoises computed values for efficiency
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries currently stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise the first four terms on the first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* a(0)=1, a(1)=1, a(2)=3, a(3)=4 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
    }

    /* if the requested index is already in cache, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) { /* allocation failure – just return 0; */
            /* In practice the build system ensures a sufficient amount of memory. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t v = 6 * cache[i - 2] - 4 * cache[i - 4];
        cache[i] = v;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
