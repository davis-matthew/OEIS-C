/*
 * A080960  Third binomial transform of A010685 (period 2: repeat 1,4).
 * https://oeis.org/A080960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2); a(0) = 1; a(1) = 7;
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External types defined by the build system */
seq_t a(idx_t n)
{
    /* Static cache to memoise computed terms */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static idx_t cache_size  = 0;      /* number of entries stored (n+1) */

    /* If the cache does not yet contain the requested term,
       extend it and compute missing values. */
    if (n >= cache_size) {
        /* Size of the new array: indices 0 … n inclusive */
        size_t new_size = (size_t)n + 1;

        /* Allocate a new buffer and copy any existing entries */
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL)        /* allocation failure – return 0 */
            return (seq_t)0;

        if (cache) {
            memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));
            free(cache);
        }

        /* Compute any terms that were not previously known */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)          new_cache[0] = (seq_t)1;
            else if (i == 1)     new_cache[1] = (seq_t)7;
            else {
                /* a(i) = 6*a(i-1) - 8*a(i-2)  */
                new_cache[i] = (seq_t)(6 * new_cache[i-1] - 8 * new_cache[i-2]);
            }
        }

        /* Install the new cache */
        cache      = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
