/*
 * A239333  Number of n X 1 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or three plus the sum of elements
 *          above it, modulo 4.
 * https://oeis.org/A239333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3), a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed terms */
static size_t cache_cap = 0;  /* number of allocated slots */

/* compute a(n) for A239333 */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
    }

    /* if already computed, return immediately */
    if (n < cache_cap)
        return cache[n];

    /* extend cache to include index n */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        /* compute all newly added terms using the recurrence */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            cache[i] = cache[i-1] * (seq_t)2 + cache[i-3] * (seq_t)2;
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
