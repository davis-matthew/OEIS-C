/*
 * A187044  Row sums of number triangle A070895.
 * https://oeis.org/A187044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + (n-2)*a(n-2) - (n-2)*a(n-3), a(0) = 1, a(1) =
 *            2, a(2) = 4
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases: return immediately for the first three indices. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* size of allocated array */
    static idx_t cached_max = (idx_t)-1;   /* highest index computed */

    /* Allocate and initialize cache on first call. */
    if (cache == NULL) {
        cache_cap = n + 1;                /* initial allocation size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out‐of‐memory; fallback to zero */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cached_max = 2;
    }

    /* Resize cache if necessary. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out‐of‐memory; fallback to zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n if needed. */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t mult = (seq_t)(i - 2);          /* (n-2) factor */
            seq_t term = cache[i - 1] * (seq_t)2; /* 2*a(i-1) */
            term += mult * cache[i - 2];          /* +(n-2)*a(i-2) */
            term -= mult * cache[i - 3];          /* -(n-2)*a(i-3) */
            cache[i] = term;
        }
        cached_max = n;
    }

    return cache[n];
}
