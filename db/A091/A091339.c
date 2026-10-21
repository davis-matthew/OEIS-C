/*
 * A091339  a(n) = a(n-1)*a(n-2) + (n-2); a(1) = 1, a(2) = 2.
 * https://oeis.org/A091339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(n)=a(n-1)*a(n-2)+n-2 for n>=3
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached sequence values. Index 0 is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* highest n computed */

/* Compute a(n) for any n >= 1.  The function is memoised so that
 * repeated calls are efficient even when the calls are out of order. */
seq_t a(idx_t n) {
    /* Initialise the cache on first use. */
    if (!cache) {
        /* Allocate space for indices 0..2 (index 0 unused). */
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Base values. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_len = 2;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[n];

    /* Resize if necessary.  Capacity is doubled until it is large
     * enough to hold the requested index. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms up to n. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* Use (seq_t)(i-2) to convert the index to the arithmetic type
         * before addition.  The multiplication is performed in seq_t. */
        cache[i] = cache[i - 1] * cache[i - 2] + (seq_t)(i - 2);
    }
    cache_len = (size_t)n;
    return cache[n];
}
