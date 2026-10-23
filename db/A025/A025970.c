/*
 * A025970  Expansion of 1/((1-2*x)*(1-4*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A025970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 188*a(n-2) + 576*a(n-3) - 576*a(n-4); Initial
 *            values: a(0)=1, a(1)=24, a(2)=388, a(3)=5376
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    /* Cached values to support arbitrary call order and O(n) computation. */
    static seq_t *cache      = NULL;   /* Pointer to the cached sequence.   */
    static idx_t cache_size   = 0;      /* Allocated size of the cache.      */
    static idx_t computed_up_to = (idx_t)3; /* Highest n for which cache is valid. */

    /* Base cases directly returned. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)388;
    if (n == 3) return (seq_t)5376;

    /* Ensure enough space and initialize base values if required. */
    if (cache == NULL || cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* In a real environment you would handle allocation failure
               appropriately. Here we simply return 0. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = (idx_t)new_size;
        /* Initialize the first four elements if they were not set yet. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)388;
        cache[3] = (seq_t)5376;
    }

    /* Compute values up to 'n' if they are not already cached. */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = 24 * cache[i-1]
                       - 188 * cache[i-2]
                       + 576 * cache[i-3]
                       - 576 * cache[i-4];
        }
        computed_up_to = n;
    }

    return cache[n];
}
