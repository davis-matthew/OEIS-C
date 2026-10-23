/*
 * A015445  Generalized Fibonacci numbers: a(n) = a(n-1) + 9*a(n-2).
 * https://oeis.org/A015445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 9*a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache with lazy initialization */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of entries currently stored */

    /* Allocate the cache for the first two terms if not already done */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();        /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;        /* a(0) */
        cache[1] = (seq_t)1;        /* a(1) */
    }

    /* Grow the cache if n is outside the current range */
    if ((size_t)n >= cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute new entries iteratively */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            /* Safety: i>=2 because cap is at least 2 before this loop */
            cache[i] = cache[i - 1] + (seq_t)9 * cache[i - 2];
        }
        cap = n + 1;
    }

    return cache[(size_t)n];
}
