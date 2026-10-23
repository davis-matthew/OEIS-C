/*
 * A118649  Row sums for A106597.
 * https://oeis.org/A118649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) with a(0) = 1, a(1) = 2,
 *            a(2) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = 2;   /* highest index pre‑computed */

    /* initialisation */
    if (cache == NULL) {
        cap   = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)  /* out of memory */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        last = 2;
    }

    /* return if already computed */
    if (n <= last)
        return cache[n];

    /* grow the cache if required */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)     /* out of memory */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* compute sequentially up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i-1]
                 + (seq_t)2 * cache[i-2]
                 - (seq_t)2 * cache[i-3];
    }
    last = n;
    return cache[n];
}
