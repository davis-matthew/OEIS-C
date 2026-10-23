/*
 * A042080  Numerators of continued fraction convergents to sqrt(564).
 * https://oeis.org/A042080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 190*a(n-4) - a(n-8); a(0)=23, a(1)=24, a(2)=71, a(3)=95,
 *            a(4)=4441, a(5)=4536, a(6)=13513, a(7)=18049 (OEIS A042080)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values of the sequence   */
    static idx_t cache_size = 0;  /* number of elements currently cached */

    /* Initialise cache with the first eight terms on the first call */
    if (cache_size == 0) {
        cache_size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)23;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)71;
        cache[3] = (seq_t)95;
        cache[4] = (seq_t)4441;
        cache[5] = (seq_t)4536;
        cache[6] = (seq_t)13513;
        cache[7] = (seq_t)18049;
    }

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Need to compute up to n; grow the cache appropriately */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence.
       We start from the first index that has not yet been filled.   */
    idx_t i_start = cache_size;
    for (idx_t i = i_start; i <= n; ++i) {
        cache[i] = cache[i - 4] * (seq_t)190 - cache[i - 8];
    }

    /* Update cache size after filling the new entries */
    cache_size = new_size;

    return cache[n];
}
