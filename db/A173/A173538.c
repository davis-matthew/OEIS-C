/*
 * A173538  a(n) = a(n-1) + a(n-2) - [a(n-2)/4] - [a(n-3)/2] - [a(n-4)/4].
 * https://oeis.org/A173538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)-Floor[a(n-2)/4]-Floor[a(n-3)/2]-Floor[a(n-4)/4]
 *            with a(0)=1, a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h> /* for memcpy */

seq_t a(idx_t n)
{
    /* Static cache for memoised terms */
    static seq_t *cache  = NULL;
    static size_t cache_size = 0;  /* number of terms stored */
    static size_t cache_cap  = 0;  /* allocated capacity */

    /* Initialise cache and base terms on first call */
    if (cache == NULL) {
        cache_cap = 8;                     /* start with room for 0 … 7 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out of memory – unlikely in tests */

        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache_size = 4;
    }

    /* If the requested term is already known, return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;         /* failure */
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t a1 = cache[(size_t)(i - 1)];
        seq_t a2 = cache[(size_t)(i - 2)];
        seq_t a3 = cache[(size_t)(i - 3)];
        seq_t a4 = cache[(size_t)(i - 4)];
        /* Use integer division truncating toward zero */
        cache[(size_t)i] = a1 + a2
                           - (a2 / (seq_t)4)
                           - (a3 / (seq_t)2)
                           - (a4 / (seq_t)4);
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
