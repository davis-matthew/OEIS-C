/*
 * A122031  a(n) = a(n - 1) + (n - 1)*a(n - 2).
 * https://oeis.org/A122031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + (n-1)*a(n-2), a(0)=1, a(1)=2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_last = 0;   /* largest index already computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                    /* hold indices 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();       /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_last = 1;
    }

    /* If requested value is already cached, return it */
    if (n <= cache_last) {
        return cache[n];
    }

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();         /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t mult = (seq_t)(i - 1) * cache[i - 2];
        cache[i] = cache[i - 1] + mult;
    }

    cache_last = n;
    return cache[n];
}
