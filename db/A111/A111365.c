/*
 * A111365  a(n) = 5*a(n-1) + 3*a(n-2) where a(0) = a(1) = 1.
 * https://oeis.org/A111365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) with a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = 0;   /* highest index already filled */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = (n + 1 > 2 ? n + 1 : 2);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        computed_up_to = 1;
    }
    /* grow the cache if necessary */
    else if (n + 1 > cache_size) {
        idx_t new_size = cache_size;
        while (new_size < n + 1) new_size <<= 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* return immediately if the value has already been computed */
    if (n <= computed_up_to)
        return cache[n];

    /* compute missing terms */
    for (idx_t i = computed_up_to + 1; i <= n; ++i)
        cache[i] = 5 * cache[i - 1] + 3 * cache[i - 2];

    computed_up_to = n;
    return cache[n];
}
