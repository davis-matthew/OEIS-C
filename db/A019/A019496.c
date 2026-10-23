/*
 * A019496  a(n) = 3*a(n-1) - 3*a(n-3) + 2*a(n-4), with a(0)=4, a(1)=11.
 * https://oeis.org/A019496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + 2*a(n-4), with a(0)=4, a(1)=11,
 *            a(2)=30, a(3)=81
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy static cache: the first four values are known a priori. */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;   /* maximum index currently stored in cache */

    /* Initialise the cache on first invocation. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 4);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)81;
        cached_max = 3;
    }

    /* If the requested index is already cached, return it immediately. */
    if (n <= cached_max) {
        return cache[(size_t)n];
    }

    /* Extend the cache to accommodate the new terms up to n. */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
    if (tmp == NULL) return (seq_t)0;   /* allocation failure */
    cache = tmp;

    /* Compute the missing terms using the recurrence. */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        cache[k] = ((seq_t)3) * cache[k - 1]
                 - ((seq_t)3) * cache[k - 3]
                 + ((seq_t)2) * cache[k - 4];
    }
    cached_max = n;

    return cache[(size_t)n];
}
