/*
 * A185308  a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + n*a(n-2) + 1.
 * https://oeis.org/A185308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + n*a(n-2) + 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms           */
    static idx_t cache_len  = 0;     /* current allocated capacity        */
    static idx_t last_computed = 0;  /* highest n already computed (>=1) */

    /* base values */
    if (n < 2) return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 2;                     /* capacity for indices 0,1 */
        cache = (seq_t *)malloc(sizeof(seq_t)*cache_len);
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        last_computed = 1;
    }

    /* extend cache if we need more room */
    if (n > last_computed) {
        /* increase capacity (double each time) until we can store index n */
        if (cache_len <= n) {
            while (cache_len <= n) cache_len <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t)*cache_len);
            if (!tmp) return (seq_t)0;  /* allocation failure */
            cache = tmp;
        }

        /* compute any new terms */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + (seq_t)i * cache[i - 2] + (seq_t)1;
        }
        last_computed = n;
    }

    return cache[n];
}
