/*
 * A158797  a(n) = a(n-1) + 36*a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A158797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + 36*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

/* Memoized recurrence computation; works for any seq_t implementation. */
seq_t a(idx_t n) {
    /* Handle base cases explicitly to avoid underflow when accessing cache. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Static cache for already computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size of cache array            */
    static idx_t cache_last = 0;   /* highest index that has been computed     */

    /* Allocate or expand cache if necessary. */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();          /* fatal if memory allocation fails */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;          /* we know n >= 2 when we reach here    */
        cache_last = 1;
    } else if (n + 1 > cache_size) {
        cache_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* If the requested index is already computed, simply return it. */
    if (n <= cache_last) {
        return cache[n];
    }

    /* Compute new values up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + (seq_t)36 * cache[i - 2];
    }
    cache_last = n;
    return cache[n];
}
