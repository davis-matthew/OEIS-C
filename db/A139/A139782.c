/*
 * A139782  Binomial transform of A077947.
 * https://oeis.org/A139782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=15
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of computed terms */
    static size_t cache_size = 0;        /* allocated size of cache      */
    static idx_t max_n = (idx_t)-1;      /* index of largest computed term */

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache_size = 8;                  /* initial allocation size */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            abort();                    /* fail if memory allocation fails */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)15;
        max_n = 3;
    }

    /* grow cache if necessary */
    if ((idx_t)n >= (idx_t)cache_size) {
        size_t new_size = (size_t)n + 1;
        if (new_size < cache_size * 2)
            new_size = cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t val = 3 * cache[i - 1] - cache[i - 3] + 3 * cache[i - 4];
            cache[i] = val;
        }
        max_n = n;
    }

    return cache[n];
}
