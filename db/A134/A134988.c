/*
 * A134988  Number of formal expressions obtained by applying iterated binary
 *          brackets to n indexed symbols x_1, ..., x_n such that: 1) each
 *          symbol appears exactly once; 2) the smallest index inside a
 *          bracket appears on the left hand side and the largest index
 *          appears on the right hand side; 3) the outer bra...
 * https://oeis.org/A134988
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2)=1, a(3)=0 a(n) = Sum_{k=2..n-2} ((k+1)*a(k+1) + a(k)) *
 *            a(n-k) for n>2 (offset 2)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache        = NULL;   /* cache[n] holds a(n) for n>=0   */
static idx_t cacheSize      = 0;     /* allocated size of cache       */
static idx_t maxComputed    = 1;     /* highest n that has been computed */

/* Allocate or grow the cache to at least size 'n+1' */
static void ensure_cache(idx_t n)
{
    if (n < cacheSize)
        return;

    idx_t newSize = (n < 32) ? 32 : n * 2;
    seq_t *newCache = (seq_t *)malloc(newSize * sizeof(seq_t));
    if (!newCache)
        exit(EXIT_FAILURE);

    if (cache) {
        /* copy existing values */
        for (idx_t i = 0; i < cacheSize; ++i)
            newCache[i] = cache[i];
        free(cache);
    }

    /* initialize new slots to 0 */
    for (idx_t i = cacheSize; i < newSize; ++i)
        newCache[i] = 0;

    cache     = newCache;
    cacheSize = newSize;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;            /* undefined for n<2, return 0 */

    ensure_cache(n);

    /* compute values that are still missing, up to n */
    for (idx_t i = maxComputed + 1; i <= n; ++i)
    {
        if (i == 2)
        {
            cache[2] = 1;
            continue;
        }
        if (i == 3)
        {
            cache[3] = 0;
            continue;
        }

        seq_t sum = 0;
        /* k runs from 2 to i-2 inclusive */
        for (idx_t k = 2; k <= i - 2; ++k)
        {
            seq_t term = ((seq_t)(k + 1)) * cache[k + 1] + cache[k];
            term *= cache[i - k];
            sum += term;
        }
        cache[i] = sum;
    }

    maxComputed = n;

    return cache[n];
}
