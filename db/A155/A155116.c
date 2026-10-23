/*
 * A155116  a(n) = 3*a(n-1) + 3*a(n-2), n>2, a(0)=1, a(1)=2, a(2)=8.
 * https://oeis.org/A155116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n‑1) + 3*a(n‑2); initial values: a(0)=1, a(1)=2,
 *            a(2)=8
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global cache for memoised values. */
static seq_t *cache = NULL;      /* pointer to stored terms */
static size_t cache_len = 0;     /* number of cached slots (size of array) */
static idx_t cached_highest = 2; /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* Quick return for the first three terms. */
    if (n <= 2) {
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)2;
        /* n == 2 */
        return (seq_t)8;
    }

    /* Initialise cache on the first call. */
    if (cache == NULL) {
        /* Ensure at least three slots for the base values. */
        cache_len = 3;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (!cache) abort();            /* memory allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cached_highest = 2;
    }

    /* Expand array if necessary to reach index n. */
    if ((size_t)n >= cache_len) {
        size_t new_len = cache_len;
        while (new_len <= (size_t)n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();             /* memory allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cached_highest + 1; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 1] + (seq_t)3 * cache[i - 2];
    }
    cached_highest = n;

    return cache[n];
}
