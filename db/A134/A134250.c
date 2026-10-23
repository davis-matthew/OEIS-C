/*
 * A134250  Expansion of x*(4+9*x-7*x^2) / ((1-x)*(1+3*x-x^2)).
 * https://oeis.org/A134250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 19, i64 n <= 38, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -2*a(n-1) + 4*a(n-2) - a(n-3) for n > 3
 * Initial values:
 *   a(1) = 4
 *   a(2) = 1
 *   a(3) = 7
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* largest index currently computed */

    if (n <= cached)          /* already known */
        return cache[n];

    idx_t old = cached;        /* last already computed index before extension */

    /* Allocate or extend the cache as necessary */
    if (old == 0) {
        /* First call: ensure at least 3 entries */
        idx_t needed = n > 3 ? n : 3;
        cache = (seq_t *)malloc((needed + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely in tests */
        cached = needed;

        /* Initialise the first three terms */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)7;
        old     = 3;
    } else if (n > cached) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cached = n;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = old + 1; i <= n; ++i) {
        cache[i] = - (seq_t)2 * cache[i - 1] + (seq_t)4 * cache[i - 2] - cache[i - 3];
    }

    return cache[n];
}
