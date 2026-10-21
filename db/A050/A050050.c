/*
 * A050050  a(n) = a(n-1)+a(m), where m=n-1-2^p and 2^p<n-1<=2^(p+1), for n >=
 *          4.
 * https://oeis.org/A050050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 195, u32 n <= 1710, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3, a(4)=4;
   for n>=5, a(n)=a(n-1)+a(n-1-2^p),
   where 2^p is the largest power of two less than n-1. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* dynamic memoisation table */
    static idx_t cache_size = 0;  /* highest index computed so far */

    /* initialise cache with the four base values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t)); /* indices 0..4 */
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
        cache_size = 4;
    }

    /* trivial case: requested value is already cached */
    if (n <= cache_size) {
        return cache[n];
    }

    /* grow the cache to hold indices up to n */
    size_t new_size = (size_t)n + 1;   /* allocate up to index n inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;         /* allocation failed: return 0 */
    cache = tmp;

    /* compute all missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* recurrence is only needed for i >= 5 */
        if (i <= 4) {
            /* already initialised */
            continue;
        }

        /* previous term index */
        idx_t prev = i - 1;

        /* largest power of two strictly less than prev */
        idx_t tmpv = prev - 1;                           /* tmpv >= 3 for i>=5 */
        idx_t L = 1ull << (63 - __builtin_clzll(tmpv));   /* 2^p */

        /* reference index m = prev - L */
        idx_t m = prev - L;

        /* a[i] = a[i-1] + a[m] */
        cache[i] = cache[i - 1] + cache[m];
    }

    cache_size = n;
    return cache[n];
}
