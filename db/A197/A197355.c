/*
 * A197355  Special values of Hermite polynomials.
 * https://oeis.org/A197355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 2*(n-1)*a(n-2), a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Computes the nth term of OEIS A197355.
 * Uses memoisation so that calls in any order run in O(n) total time. */
seq_t a(idx_t n)
{
    /* static cache of already computed values: a[0] .. a[cache_size-1] */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */

    /* Initialise cache for the first two terms when needed. */
    if (cache_size == 0) {
        /* Allocate space for two terms. */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed – return 0 */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        cache[1] = (seq_t)8;             /* a(1) = 8 */
        cache_size = 2;
    }

    /* If the requested index is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache up to n (inclusive). */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;           /* allocation failed – return 0 */
    cache = new_cache;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t term1 = prev1 * (seq_t)8;
        seq_t term2 = prev2 * ((seq_t)2 * (seq_t)(i - 1));
        cache[i] = term1 + term2;
    }

    cache_size = n + 1;
    return cache[n];
}
