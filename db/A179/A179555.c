/*
 * A179555  a(1)=10; a(n) = a(n-1)*10 - 1.
 * https://oeis.org/A179555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 10; a(n) = a(n-1)*10 - 1 for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        cache[0] = (seq_t)10;
        cache_size = 1;
    }

    /* return cached value if already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* grow cache to accommodate up to n */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    for (size_t i = cache_size; i < n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)10 - (seq_t)1;
    }
    cache_size = n;

    return cache[n - 1];
}
