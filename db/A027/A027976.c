/*
 * A027976  n-th diagonal sum of right justified array T given by A027960.
 * https://oeis.org/A027976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) - a(n-5) with a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=6, a(4)=10
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;

/* Returns the n-th term of sequence A027976. */
seq_t a(idx_t n)
{
    /* Ensure the cache is initialized with base values. */
    if (!cache) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)10;
        cache_max = 4;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cache_max)
        return cache[n];

    /* Resize the cache to hold the new range. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;           /* allocation failed */
    cache = new_cache;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                 - cache[i-4] - cache[i-5];
    }
    cache_max = n;
    return cache[n];
}
