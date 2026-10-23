/*
 * A162723  a(n) = 9 a (n-1)-26 a(n-2) +24 a(n-3) (n >= 3) with a(0) =a(1)=1,
 *          a(2)=2.
 * https://oeis.org/A162723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162723
   a(n) = 9*a(n-1) - 26*a(n-2) + 24*a(n-3)
   a(0) = a(1) = 1, a(2) = 2
   This memoised recurrence returns the correct term for every n≥0. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;           /* pointer to computed values   */
    static idx_t cache_cap = 0;           /* allocated size (elements)   */
    static idx_t cache_up_to = (idx_t)-1; /* highest index computed so far */

    /* initialise cache on first call */
    if (cache_up_to == (idx_t)-1) {
        cache_cap = 3;   /* store a(0), a(1), a(2) at start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure – unlikely in test */
        cache[0] = 1;  cache[1] = 1;  cache[2] = 2;
        cache_up_to = 2;
    }

    /* If the requested term is already cached, return it immediately */
    if (n <= cache_up_to)
        return cache[n];

    /* Ensure the cache can hold indices up to n */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        /* double the capacity until it is large enough */
        while (required > cache_cap) {
            cache_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely */
        cache = tmp;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        if (i == 0 || i == 1)
            cache[i] = 1;
        else if (i == 2)
            cache[i] = 2;
        else
            cache[i] = 9 * cache[i - 1] - 26 * cache[i - 2] + 24 * cache[i - 3];
    }

    cache_up_to = n;
    return cache[n];
}
