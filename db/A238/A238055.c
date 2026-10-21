/*
 * A238055  a(n) = (13*3^n - 1)/2.
 * https://oeis.org/A238055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13*3^n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=6, a(n)=3*a(n-1)+1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;       /* number of elements initialized */
    static size_t cache_cap  = 0;       /* allocated capacity */

    /* expand capacity if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* initialise first term if cache empty */
    if (cache_size == 0) {
        cache[0] = 6;
        cache_size = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i - 1] * 3 + 1;
    }
    cache_size = n + 1;

    return cache[n];
}
