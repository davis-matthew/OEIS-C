/*
 * A098247  First differences of Chebyshev polynomials S(n,227) = A098245(n)
 *          with Diophantine property.
 * https://oeis.org/A098247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 227*a(n-1) - a(n-2); a(0)=1, a(1)=226
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A098247. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;      /* array of terms */
    static idx_t cache_size = 0;     /* current number of stored terms */

    /* Initialise cache with first two terms if not yet done. */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) { /* out of memory; fall back to minimal behaviour */
            return 0;
        }
        cache[0] = 1;
        cache[1] = 226;
    }

    /* Grow the cache up to n. */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) { /* out of memory; keep current cache */
            return 0;
        }
        cache = tmp;
        /* Compute missing terms using the linear recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] * 227 - cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
