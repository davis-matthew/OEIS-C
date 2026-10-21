/*
 * A072042  a(n+2) = a(n+1)*a(n)*(1+1/n), a(1)=a(2)=1.
 * https://oeis.org/A072042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = a(2) = 1, a(n) = a(n-1) * a(n-2) * (n-1) / (n-2) for n ≥
 *            3 (OEIS A072042)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 2)
        return (seq_t)1;

    /* Static cache of previously computed terms a(1)...a(len) */
    static seq_t *cache = NULL;      /* allocated array */
    static size_t  len  = 0;        /* number of terms stored */
    static size_t  cap  = 0;        /* allocated capacity */

    /* Initialise the cache with the first two terms on first call */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* Allocation failed: return 0 as sentinel */
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)1;  /* a(2) */
        len  = 2;
    }

    /* Grow the allocation if necessary */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap : 2;
        while (newcap < (size_t)n)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            len = cap = 0;
            return (seq_t)0;   /* Allocation failed: return 0 as sentinel */
        }
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (size_t k = len + 1; k <= (size_t)n; ++k) {
        /* a(k) = a(k-1) * a(k-2) * (k-1) / (k-2)  */
        seq_t prod = cache[k-2] * cache[k-3];       /* a(k-1) * a(k-2)       */
        prod *= (seq_t)(k-1);                      /* * (k-1)               */
        prod /= (seq_t)(k-2);                      /* / (k-2)  (exact)      */
        cache[k-1] = prod;
    }
    len = (size_t)n;

    return cache[n-1];
}
