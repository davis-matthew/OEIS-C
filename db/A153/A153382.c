/*
 * A153382  a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4), a(0)=0, a(1)=8,
 *          a(2)=10, a(3)=18.
 * https://oeis.org/A153382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4); Initial values:
 *            a(0)=0, a(1)=8, a(2)=10, a(3)=18
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values   */
    static idx_t cache_len = 0;   /* number of cached entries */

    /* initialise the first four terms once */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)18;
    }

    /* already computed? return directly */
    if (n < cache_len) return cache[n];

    /* remember old size before possible reallocation */
    idx_t old_len = cache_len;

    /* grow cache if necessary */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = old_len; i <= n; ++i) {
#ifdef __cplusplus          /* arbitrary‑precision (C++ build) */
        seq_t v = cache[i-1] + cache[i-2] + cache[i-2]
                           - cache[i-3] - cache[i-4];
        cache[i] = v;
#else                       /* integral types (C build) */
        unsigned __int128 tmp = (unsigned __int128)cache[i-1];
        tmp += (unsigned __int128)cache[i-2] * 2;
        tmp -= cache[i-3];
        tmp -= cache[i-4];
        cache[i] = (seq_t)tmp;
#endif
    }

    /* the cache now contains terms up to n */
    cache_len = n + 1;

    return cache[n];
}
