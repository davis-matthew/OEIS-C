/*
 * A226309  a(n) = a(n-1) + a(n-2) + a(n-3) + 2*a(n-4) with a(0)=2, a(1)=1,
 *          a(2)=5, a(3)=10.
 * https://oeis.org/A226309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 2*a(n-4), a(0)=2, a(1)=1,
 *            a(2)=5, a(3)=10
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization array */
    static idx_t cache_size = 0;   /* number of elements stored */

    /* Initialise the first four terms. */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)10;
    }

    /* If the required index is already computed, return it. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache to accommodate up to index n. */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* allocation failure */
    cache = tmp;

    /* Compute all new terms using the recurrence. */
    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + ((seq_t)2) * cache[i-4];
    }

    return cache[n];
}
