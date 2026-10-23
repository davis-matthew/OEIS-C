/*
 * A021804  Expansion of 1/((1-x)(1-4x)(1-6x)(1-7x)).
 * https://oeis.org/A021804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1)-111*a(n-2)+262*a(n-3)-168*a(n-4), a(0)=1,
 *            a(1)=18, a(2)=213, a(3)=2098
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms (converted modulo seq_t size) */
    static const seq_t init[4] = { 1, 18, 213, 2098 };

    /* Return the first four values directly */
    if (n < 4)
        return init[n];

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Allocate cache and load the first four terms on first use */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache_size = 4;
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
    }

    /* Extend the cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)  /* allocation failure – return 0 as a safe fallback */
            return 0;
        cache = new_cache;
        cache_size = new_size;

        /* Compute new terms using the recurrence */
        for (idx_t i = old_size; i <= n; ++i) {
            cache[i] = (seq_t)18 * cache[i - 1]
                     - (seq_t)111 * cache[i - 2]
                     + (seq_t)262 * cache[i - 3]
                     - (seq_t)168 * cache[i - 4];
        }
    }

    return cache[n];
}
