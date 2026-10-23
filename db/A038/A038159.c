/*
 * A038159  a(n) = n*a(n-1) + 1, a(0) = 2.
 * https://oeis.org/A038159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 1, a(0) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

/* Memoized computation of A038159 */
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t  cache_last = (idx_t)-1; /* highest index computed */

    /* Ensure base term a(0)=2 is available */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)2;
        cache_last = 0;
    }

    /* Expand cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* cache[i] = i * cache[i-1] + 1 */
        cache[i] = (seq_t)i * cache[i - 1] + (seq_t)1;
    }
    cache_last = n;

    return cache[n];
}
