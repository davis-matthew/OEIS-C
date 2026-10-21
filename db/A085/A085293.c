/*
 * A085293  Product of Lucas (A000204) and a Pell Companion series (A002203).
 * https://oeis.org/A085293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 2*a(n-3) - a(n-4) (n > 4) with
 *            a(1)=2, a(2)=18, a(3)=56, a(4)=238. (Offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    /* Ensure we have the first four terms initialized */
    if (cached_n == 0) {
        cache = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed; caller handles */
        cache[0] = 2;     /* a(1) */
        cache[1] = 18;    /* a(2) */
        cache[2] = 56;    /* a(3) */
        cache[3] = 238;   /* a(4) */
        cached_n = 4;
    }

    /* Expand cache if needed */
    if (n > cached_n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed; caller handles */
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            /* i is 1-based index; cache[i-2] is a(i-1), etc. */
            cache[i - 1] = 2 * cache[i - 2] +
                           7 * cache[i - 3] +
                           2 * cache[i - 4] -
                           cache[i - 5];
        }
        cached_n = n;
    }

    /* return the computed (or cached) value */
    return cache[n - 1];
}
