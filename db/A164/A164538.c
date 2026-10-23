/*
 * A164538  a(n) = 10*a(n-1) - 23*a(n-2) for n > 1; a(0) = 5, a(1) = 33.
 * https://oeis.org/A164538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2) for n > 1; a(0) = 5, a(1) = 33
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of valid entries in cache */
    static idx_t cache_cap = 0;      /* allocated size of cache */

    /* initialize cache if needed */
    if (cache == NULL) {
        cache_cap = 2;                      /* small initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0; /* allocation failure handled silently */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)33;
        cache_len = 2;
    }

    /* expand cache if the requested index is out of range */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n + 1) {
            /* double the capacity each time (geometric growth) */
            new_cap = new_cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;     /* allocation failure handled silently */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)5;
        } else if (i == 1) {
            cache[1] = (seq_t)33;
        } else {
            /* recurrence: a(i) = 10*a(i-1) - 23*a(i-2) */
            cache[i] = (seq_t)((seq_t)10 * cache[i - 1] - (seq_t)23 * cache[i - 2]);
        }
    }
    cache_len = n + 1;  /* all indices up to n are now valid */

    return cache[n];
}
