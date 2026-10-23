/*
 * A020534  a(n) = 8th Fibonacci polynomial evaluated at 2^n.
 * https://oeis.org/A020534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 170*a(n-1) - 5712*a(n-2) + 43520*a(n-3) - 65536*a(n-4);
 *            Initial values: a(0)=21, a(1)=408, a(2)=23184, a(3)=2298912
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static idx_t  cache_cap   = 0;

    /* initialise cache with first four terms if needed */
    if (!cache) {
        cache_cap   = 4;
        cache_size  = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)21;
        cache[1] = (seq_t)408;
        cache[2] = (seq_t)23184;
        cache[3] = (seq_t)2298912;
    }

    /* if the requested value is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* grow cache as necessary and compute missing values */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* compute a(k) using the recurrence with only unsigned ops */
        seq_t term1 = (seq_t)170 * cache[k-1];
        seq_t term2 = (seq_t)43520 * cache[k-3];
        seq_t sub   = (seq_t)5712 * cache[k-2];
        sub += (seq_t)65536 * cache[k-4];
        seq_t val = term1 + term2 - sub;

        /* ensure we have enough space in the cache */
        if (k >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap <= k)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[k] = val;
    }

    cache_size = n + 1; /* we just filled up to index n */
    return cache[n];
}
