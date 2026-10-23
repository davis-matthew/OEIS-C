/*
 * A182895  Number of (1,0)-steps at level 0 in all weighted lattice paths in
 *          L_n.
 * https://oeis.org/A182895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182895
   Formula (recurrence):
      a(0)=0, a(1)=1, a(2)=3, a(3)=7
      for n≥4:  a(n)=2*a(n-1)+a(n-2)+2*a(n-3)−a(n-4)
   This follows from the generating function
      z(1+z)/[(1+z+z^2)(1-3z+z^2)].
   The implementation keeps a cache so that any call can be answered
   in O(n) time relative to the largest n seen so far. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* small constants as seq_t for safety */
    static const seq_t zero   = (seq_t)0;
    static const seq_t one    = (seq_t)1;
    static const seq_t two    = (seq_t)2;
    static const seq_t three  = (seq_t)3;
    /* store already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of entries cached */

    /* return directly for the very first four indices */
    if (n < 4) {
        switch (n) {
            case 0: return zero;
            case 1: return one;
            case 2: return three;
            case 3: return (seq_t)7;
        }
    }

    /* initialise cache with the first four terms if not yet done */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0] = zero;
        cache[1] = one;
        cache[2] = three;
        cache[3] = (seq_t)7;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* extend the cache to hold index n */
    cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));

    /* compute values from the first uncached index up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: 2*a(i-1) + a(i-2) + 2*a(i-3) - a(i-4) */
        seq_t val = cache[i-1] * two
                  + cache[i-2]
                  + cache[i-3] * two
                  - cache[i-4];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
