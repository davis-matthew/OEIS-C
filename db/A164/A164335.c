/*
 * A164335  Quintic recurrence sequence a(0) = 1, a(n) = n*a(n-1)^5.
 * https://oeis.org/A164335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = n*a(n-1)^5
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *cache     = NULL;
    static idx_t cacheSize = 0;

    if (n < cacheSize) return cache[n];

    /* Resize cache to hold 0 … n */
    idx_t newSize = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
    if (!tmp && newSize > 0) abort();
    cache = tmp;

    /* Compute new terms */
    for (idx_t i = cacheSize; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else
        {
            seq_t p = seq_pow(cache[i-1], 5);
            cache[i] = (seq_t)i * p;
        }
    }

    cacheSize = newSize;
    return cache[n];
}
