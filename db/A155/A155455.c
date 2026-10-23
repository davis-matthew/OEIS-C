/*
 * A155455  a(n) = 5*a(n-1)+16*a(n-2), n>1 ; a(0)=0, a(1)=1.
 * https://oeis.org/A155455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A155455: a(n) = 5*a(n-1)+16*a(n-2),  a(0)=0, a(1)=1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* static cache for dynamic programming */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache for indices 0 and 1 */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed – return 0 as sentinel */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* if n is already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend cache to include n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) { /* allocation failed – return 0 as sentinel */
        return (seq_t)0;
    }
    cache = new_cache;

    /* compute new entries using the linear recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1] + 16 * cache[i - 2];
    }

    cache_size = new_size;
    return cache[n];
}
