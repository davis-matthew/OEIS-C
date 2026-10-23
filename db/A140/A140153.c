/*
 * A140153  a(1)=1, a(n) = a(n-1) + n^3 if n odd, a(n) = a(n-1) + n^1 if n is
 *          even.
 * https://oeis.org/A140153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = a(n-1) + n^3 if n is odd, else a(n) = a(n-1) +
 *            n (even)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 430, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary order of calls */
    static seq_t *cache = NULL;   /* index 0 holds a(1) */
    static idx_t cache_n = 0;     /* number of elements cached */

    if (n == 0) return 0; /* undefined for n < 1; safe guard */

    /* initialise cache if first call */
    if (cache_n == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = 1;      /* a(1) */
        cache_n = 1;
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_n) return cache[n - 1];

    /* Extend the cache to at least size n */
    size_t new_size = (size_t)n;
    if (new_size < cache_n * 2) new_size = cache_n * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    cache = tmp;

    /* Compute missing terms */
    for (idx_t k = cache_n + 1; k <= n; ++k) {
        seq_t add = (k & 1UL)   /* odd? */
                    ? (seq_t)k * (seq_t)k * (seq_t)k
                    : (seq_t)k;
        cache[k - 1] = cache[k - 2] + add;
    }

    cache_n = n;                       /* update cached size */
    return cache[n - 1];
}
