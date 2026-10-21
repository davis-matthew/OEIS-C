/*
 * A084137  Binomial transform of A084136.
 * https://oeis.org/A084137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) - 8*a(n-3) with a(0)=1, a(1)=2,
 *            a(2)=8
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache for memoization. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of elements stored */
    static size_t cache_capacity = 0;  /* allocated capacity */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_capacity = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        /* base cases */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache_size = 3;
    }

    /* If we already have the requested term, return it. */
    if ((size_t)n < cache_size) {
        return cache[n];
    }

    /* Ensure we have enough capacity for n+1 elements. */
    if (n + 1 > cache_capacity) {
        size_t new_capacity = cache_capacity;
        while (new_capacity <= (size_t)n) new_capacity *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_capacity);
        cache_capacity = new_capacity;
    }

    /* Compute terms up to n using the recurrence. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        cache[i] = 6*a1 - 4*a2 - 8*a3;
    }
    cache_size = (size_t)n + 1;

    return cache[n];
}
