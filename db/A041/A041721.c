/*
 * A041721  Denominators of continued fraction convergents to sqrt(380).
 * https://oeis.org/A041721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 78*a(n-2) - a(n-4), a(0)=1, a(1)=2, a(2)=77, a(3)=156 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first four terms if needed */
    if (cache_len == 0) {
        /* allocate space for at least four terms */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* safety, should never happen */
        cache_len = 4;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)77;
        cache[3] = (seq_t)156;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to accommodate up to n */
    idx_t new_cap = n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) return (seq_t)0;   /* safety */

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 78*a(i-2) - a(i-4) */
        cache[i] = (seq_t)78 * cache[i - 2] - cache[i - 4];
    }
    cache_len = new_cap;
    return cache[n];
}
