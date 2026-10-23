/*
 * A166304  Third trisection of A022998.
 * https://oeis.org/A166304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = A022998(3*n+2)
 * Recurrence: a(0)=4, a(1)=5, a(2)=16, a(3)=11,
 *             a(n) = 2*a(n-2) - a(n-4)  for n >= 4
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t count = 0;   /* number of entries stored */

    size_t target = (size_t)n;
    /* Initialise cache with first four values on first call */
    if (cache == NULL) {
        cap   = 4;
        count = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure; may replace */
        cache[0] = 4;
        cache[1] = 5;
        cache[2] = 16;
        cache[3] = 11;
    }

    /* If the requested term is already cached, return it */
    if (target < count) {
        return cache[target];
    }

    /* Grow the cache if necessary */
    if (target + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 4;
        while (new_cap < target + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure; may replace */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to target using the recurrence */
    for (size_t i = count; i <= target; ++i) {
        cache[i] = cache[i - 2] + cache[i - 2] - cache[i - 4];
    }
    count = target + 1;
    return cache[target];
}
