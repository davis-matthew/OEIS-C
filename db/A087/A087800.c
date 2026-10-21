/*
 * A087800  a(n) = 12*a(n-1) - a(n-2), with a(0) = 2 and a(1) = 12.
 * https://oeis.org/A087800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-1) - a(n-2), a(0) = 2, a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocation capacity */
    static size_t size  = 0;   /* number of cached entries */

    /* initialise cache with base cases */
    if (!cache) {
        cap   = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)12;
        size   = 2;
    }

    /* if requested value already computed, return it */
    if ((size_t)n < size)
        return cache[(size_t)n];

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap <= (size_t)n + 1)
            newcap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        cap   = newcap;
    }

    /* compute missing values up to n */
    for (size_t i = size; i <= (size_t)n; ++i)
        cache[i] = (seq_t)12 * cache[i - 1] - cache[i - 2];

    size = (size_t)n + 1;
    return cache[(size_t)n];
}
