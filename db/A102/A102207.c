/*
 * A102207  a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3) with a(0) = 4, a(1) = 17, a(2)
 *          = 65.
 * https://oeis.org/A102207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3) with a(0)=4, a(1)=17,
 *            a(2)=65
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_last = (idx_t)-1;
    static size_t cache_size = 0;

    /* Return base values directly if small */
    if (n == 0) return 4;
    if (n == 1) return 17;
    if (n == 2) return 65;

    size_t target = (size_t)n;             /* target index as size_t */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure handling */
        cache[0] = 4;
        cache[1] = 17;
        cache[2] = 65;
        cache_last = 2;
    }

    /* If requested value already in cache, return it */
    if (target <= (size_t)cache_last)
        return cache[target];

    /* Grow the cache buffer if needed */
    if (target >= cache_size) {
        size_t new_size = cache_size ? cache_size : 3;
        while (new_size <= target)
            new_size *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms from cache_last+1 up to target */
    for (size_t i = (size_t)cache_last + 1; i <= target; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        cache[i] = (seq_t)5 * t1 - (seq_t)5 * t2 + t3;
    }
    cache_last = target;
    return cache[target];
}
