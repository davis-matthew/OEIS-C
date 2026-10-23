/*
 * A022401  Fibonacci sequence beginning 1, 31.
 * https://oeis.org/A022401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 31
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_size = 0;  /* number of terms stored (cache_size >= 2 after init) */

seq_t a(idx_t n)
{
    /* lazily initialise the cache with the first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)31;
        cache_size = 2;
    }

    /* if the value is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* enlarge the cache to hold terms up to n */
    idx_t new_size = n + 1u;          /* +1 because indices start at 0 */
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failure */
        cache = tmp;
        /* keep the old size for the computation loop */
        idx_t old_size = cache_size;
        cache_size = new_size;

        /* compute the missing terms */
        for (idx_t i = old_size; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 2];
    }

    return cache[n];
}
