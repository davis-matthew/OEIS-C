/*
 * A098578  a(n) = Sum_{k=0..floor(n/4)} C(n-3*k,k+1).
 * https://oeis.org/A098578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-4) + 1 (a(0)=0, a(1)=1, a(2)=2, a(3)=3)
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 67, u64 n <= 136,
 *            u128 n <= 274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A098578: a(n) = Sum_{k=0..floor(n/4)} C(n-3*k, k+1) */
/* The recurrence a(n) = a(n-1) + a(n-4) + 1 is used for fast O(n) computation. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;   /* array of a(0)..a(max) */
    static idx_t  max   = 0;      /* last index stored in cache        */

    /* Initialise cache with the first four terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();   /* OOM protection */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        max = 3;
    }

    /* Return cached value if already computed */
    if (n <= max)
        return cache[n];

    /* Grow the cache to accommodate indices up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing values using the recurrence */
    for (idx_t i = max + 1; i <= n; ++i) {
        if (i < 4) /* safety, though should never happen */
            continue;
        cache[i] = cache[i - 1] + cache[i - 4] + 1;
    }
    max = n;
    return cache[n];
}
