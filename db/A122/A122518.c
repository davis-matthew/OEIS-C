/*
 * A122518  G.f.: 1/(1 -2 x^3 - x^4 + x^5).
 * https://oeis.org/A122518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-3) + a(n-4) - a(n-5) for n ≥ 5 with initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=1
 * Range:     i8 n <= 18, i16 n <= 38, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds consecutively computed values up to the largest
       index requested so far.  The array grows as needed. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of the array */
    static idx_t last = (idx_t)-1; /* largest index currently stored */

    /* For indices below the offset, return the hard‑coded values. */
    if (n < 5) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)2;
            case 4: return (seq_t)1;
        }
    }

    /* Lazy allocation of the cache with the known initial terms. */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)1;
        last = 4;
    }

    /* If the requested index is already in the cache, return it. */
    if (n <= last)
        return cache[n];

    /* Grow the cache if necessary.  Simple strategy: allocate exactly
       enough space for indices 0 … n. */
    if ((size_t)n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = (size_t)n + 1;
    }

    /* Compute the missing values sequentially using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 3] + cache[i - 4] - cache[i - 5];
    }
    last = n;

    return cache[n];
}
