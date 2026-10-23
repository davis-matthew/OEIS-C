/*
 * A154999  a(n) = 7*a(n-1) + 42*a(n-2), n>2; a(0)=1, a(1)=1, a(2)=13.
 * https://oeis.org/A154999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 42*a(n-2), a(0)=1, a(1)=1, a(2)=13
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n‑th term of OEIS A154999. */
seq_t a(idx_t n)
{
    /* Static cache for memoization (thread‑unsafe, but permissible). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure cache holds at least three seed values. */
    if (cache_len < 3) {
        size_t init_len = (size_t)3;
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 13;
        cache_len = init_len;
    }

    /* Return immediately if the requested term is already cached. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow the cache to include term n. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return 0; /* out of memory */
    cache = tmp;
    cache_len = new_len;

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = 3; i <= (size_t)n; ++i) {
        cache[i] = 7 * cache[i - 1] + 42 * cache[i - 2];
    }

    return cache[n];
}
