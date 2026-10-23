/*
 * A152174  a(n) = -2*a(n-1)+4*a(n-2), n>1 ; a(0) = 1, a(1) = -4.
 * https://oeis.org/A152174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1)+4*a(n-2), a(0)=1, a(1)=-4
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A152174.
 * The sequence satisfies the recurrence
 *   a(n) = -2*a(n-1) + 4*a(n-2),   n > 1
 * with initial values a(0) = 1 and a(1) = -4.
 * The result is memoised so that calls in any order are
 * efficient.
 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;

    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;                 /* allocation failure – return 0 (unused case) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-4;
    }

    /* Extend cache if requested n is beyond current size */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;           /* allocation failure */
        cache = new_cache;

        /* Compute missing terms using recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = -2 * cache[i-1] + 4 * cache[i-2];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
