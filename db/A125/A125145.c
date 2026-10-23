/*
 * A125145  a(n) = 3a(n-1) + 3a(n-2). a(0) = 1, a(1) = 4.
 * https://oeis.org/A125145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=4; a(n)=3*a(n-1)+3*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache for computed terms */
    static size_t cap   = 0;      /* allocated capacity of the cache */
    static idx_t  len   = 0;      /* number of terms already filled */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cap = 2;
        len = 2;
    }

    /* Grow cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap : 2;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((idx_t)(n + 1) > len) {
        for (idx_t i = len; i <= n; ++i)
            cache[i] = (seq_t)3 * cache[i - 1] + (seq_t)3 * cache[i - 2];
        len = n + 1;
    }

    return cache[n];
}
