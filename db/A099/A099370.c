/*
 * A099370  Chebyshev polynomial of the first kind, T(n,x), evaluated at x=33.
 * https://oeis.org/A099370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*a(n-1) - a(n-2) with a(0) = 1, a(1) = 33. Index offset
 *            is 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: array of previously computed terms.
       cached_n holds the number of cached terms (0 .. cached_n-1).   */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;      /* number of terms cached */
    static size_t cache_cap = 0;    /* current allocation size */

    /* Initialisation on first call.  The first two terms are always needed. */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)33;
        cached_n = 2;
    }

    /* If the requested term is already cached, simply return it. */
    if (n < cached_n)
        return cache[(size_t)n];

    /* Otherwise, extend the cache up to n (inclusive).  Resize if necessary. */
    while (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Out‑of‑memory: fallback to computing the terms on the fly
               (should never happen in the intended test harness).   */
            for (idx_t i = cached_n; i <= n; ++i) {
                cache[i] = (66 * cache[i - 1]) - cache[i - 2];
            }
            return cache[(size_t)n];
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = cached_n; i <= n; ++i) {
        cache[(size_t)i] = (66 * cache[(size_t)(i - 1)]) - cache[(size_t)(i - 2)];
    }
    cached_n = n + 1;

    return cache[(size_t)n];
}
