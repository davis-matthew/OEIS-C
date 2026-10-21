/*
 * A099526  Expansion of 1/(1-2x-3x^4).
 * https://oeis.org/A099526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-4), a(0)=1, a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache of computed terms */
    static idx_t cache_len = 0;     /* cache size */

    /* First run: initialise the four base terms */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
    }

    /* Return a cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Resize the cache and compute missing terms */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */

    cache = tmp;
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)3 * cache[i - 4];
    }
    cache_len = new_len;

    return cache[n];
}
