/*
 * A158761  a(n)=a(n-2)+64*a(n-3) with a(0)=1, a(1)=4, a(3)=16.
 * https://oeis.org/A158761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 64*a(n-3) Recurrence base values: a(0)=1,
 *            a(1)=4, a(2)=16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure: return zero */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
    }

    /* If requested term is within current cache, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to accommodate up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;  /* allocation failure */
    cache = new_cache;
    cache_size = new_size;

    /* Compute missing terms using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        cache[i] = cache[i - 2] + ((seq_t)64 * cache[i - 3]);
    }

    return cache[n];
}
