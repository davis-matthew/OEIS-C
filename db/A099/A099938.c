/*
 * A099938  Consider the sequence of circles C0, C1, C2, C3 ..., where C0 is a
 *          half-circle of radius 1. C1 is the largest circle that fits into
 *          C0 and has radius 1/2. C(n+1) is the largest circle that fits
 *          inside C0 but outside C(n), etc. Sequence gives the curvatures
 *          (reciprocals of the radii) of the circles.
 * https://oeis.org/A099938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3), with a(1)=2, a(2)=4,
 *            a(3)=18
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc/realloc */

seq_t a(idx_t n)
{
    /* Static cache of computed values; 0‑based indexing: cache[0]=a(1) etc. */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;

    /* Initialise the first three terms on first call. */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)4;   /* a(2) */
        cache[2] = (seq_t)18;  /* a(3) */
        cache_len = 3;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend the cache up to the requested index. */
    idx_t old_len = cache_len;
    cache_len = n;
    cache = (seq_t*)realloc(cache, (size_t)cache_len * sizeof(seq_t));

    for (idx_t i = old_len; i < n; ++i) {
        /* i is the 0‑based index of a(i+1).  Compute using recurrence. */
        seq_t val = (seq_t)7 * cache[i - 1] - (seq_t)7 * cache[i - 2] + cache[i - 3];
        cache[i] = val;
    }

    return cache[n - 1];
}
