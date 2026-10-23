/*
 * A159670  Numerator of Hermite(n, 13/20).
 * https://oeis.org/A159670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 13, a(n) = 13*a(n-1) - 200*(n-1)*a(n-2) for n ≥
 *            2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of sequence A159670.
   The function is memoised and works for any order of calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store previously computed terms */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static idx_t  size    = 0;      /* current size of the cache */

    /* Initialise the cache with the two base terms if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL)          /* unlikely but keep it safe */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        size    = 2;
    }

    /* Expand the cache if it does not yet contain the requested index */
    if (n >= size) {
        size_t new_size = (size_t)n + 1;   /* cast n to size_t for allocation */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)                     /* not expected in tests */
            return (seq_t)0;
        cache = tmp;

        /* Fill in missing entries using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            seq_t term1 = (seq_t)13 * cache[i - 1];
            seq_t factor = (seq_t)200 * (seq_t)(i - 1);
            seq_t term2 = factor * cache[i - 2];
            cache[i] = term1 - term2;
        }
        size = (idx_t)new_size;
    }

    return cache[n];
}
