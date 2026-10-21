/*
 * A221082  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor.
 * https://oeis.org/A221082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=24, a(n)=12*a(n-1)-9*a(n-2) for n≥3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;   /* last computed index (≥0) */

    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0, but safe */

    /* expand capacity if needed */
    if ((size_t)n + 1 > cache_cap) {
#ifdef __cplusplus
        /* allocate new larger array */
        seq_t *new_cache = new seq_t[(size_t)n + 1];
        /* copy old values */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        /* free old array */
        delete[] cache;
        cache = new_cache;
        cache_cap = (size_t)n + 1;
#else
        void *p = realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!p) return (seq_t)0;          /* allocation failure (unlikely) */
        cache = (seq_t *)p;
        cache_cap = (size_t)n + 1;
#endif
    }

    /* compute missing terms */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)24;
        } else {
            cache[i] = (seq_t)12 * cache[i - 1] - (seq_t)9 * cache[i - 2];
        }
    }
    cache_last = n;
    return cache[n];
}
