/*
 * A088992  Derangement numbers d(n,5) where d(n,k) = k(n-1)(d(n-1,k) +
 *          d(n-2,k)), with d(0,k) = 1 and d(1,k) = 0.
 * https://oeis.org/A088992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   d(n,5) = 5*(n-1)*(d(n-1,5)+d(n-2,5)), d(0,5)=1, d(1,5)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;          /* array of terms */
    static idx_t cache_size = 0;         /* allocated size of cache */
    static idx_t max_computed = 0;       /* highest index that has been computed */

    /* initialisation on first call */
    if (cache == NULL) {
        cache_size = 16;                  /* initial capacity */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;             /* out of memory, return 0 as default */

        /* set base values */
        cache[0] = (seq_t)1;              /* d(0) */
        if (cache_size > 1)
            cache[1] = (seq_t)0;          /* d(1) */
        max_computed = 1;
    }

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n)
            new_size <<= 1;               /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute new terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t mul1 = (seq_t)(i - 1);
        seq_t mul2 = (seq_t)5;
        cache[i] = mul2 * mul1 * sum;    /* 5*(i-1)*(d(i-1)+d(i-2)) */
    }
    max_computed = n;

    return cache[n];
}
