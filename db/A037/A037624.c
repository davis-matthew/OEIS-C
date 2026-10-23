/*
 * A037624  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,0,3.
 * https://oeis.org/A037624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ⌊(203/999)·10ⁿ⌋ Recurrence: a(1)=2, a(2)=20, a(3)=203,
 *            a(4)=2032; a(n) = 10·a(n-1) + a(n-3) – 10·a(n-4) for n≥5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] == a(i+1) */
static idx_t cacheSize = 0;   /* number of entries cached */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, return 0 as sentinel */

    /* Expand cache if necessary */
    if (n > cacheSize) {
        idx_t newSize = n;
        seq_t *newCache = (seq_t *)malloc(newSize * sizeof(seq_t));
        if (!newCache) abort();      /* out of memory */

        /* Copy old cache */
        for (idx_t i = 0; i < cacheSize; ++i)
            newCache[i] = cache[i];

        /* Initialize missing initial terms */
        if (cacheSize < 1 && newSize > 0) newCache[0] = 2;
        if (cacheSize < 2 && newSize > 1) newCache[1] = 20;
        if (cacheSize < 3 && newSize > 2) newCache[2] = 203;
        if (cacheSize < 4 && newSize > 3) newCache[3] = 2032;

        /* Apply recurrence for remaining terms */
        idx_t start = (cacheSize < 4) ? 4 : cacheSize;
        for (idx_t i = start; i < newSize; ++i) {
            seq_t term = 10 * newCache[i-1]  /* 10·a(i)      */
                        + newCache[i-3]      /* + a(i-2)     */
                        - 10 * newCache[i-4];/* -10·a(i-3)   */
            newCache[i] = term;
        }

        /* Replace the old cache */
        free(cache);
        cache = newCache;
        cacheSize = newSize;
    }

    return cache[n-1];
}
