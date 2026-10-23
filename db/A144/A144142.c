/*
 * A144142  a(n) = Hermite(n,3).
 * https://oeis.org/A144142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n,3) Recurrence: a(0)=1, a(1)=6, a(n) = 6*a(n-1)
 *            - 2*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 15, i64 n <= 26, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static size_t used = 0;  /* number of terms computed */

    /* handle offset 0 and 1 explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* allocate or grow cache if necessary */
    if (cache == NULL || (size_t)(n + 1) > cap) {
        size_t newcap = cap ? cap << 1 : 4;
        if ((size_t)(n + 1) > newcap)
            newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort(); /* memory failure */
        cache = tmp;
        cap = newcap;
    }

    /* initialise first two terms if cache was just created */
    if (used == 0) {
        cache[0] = (seq_t)1;
        used = 1;
    }
    if (used == 1) {
        cache[1] = (seq_t)6;
        used = 2;
    }

    /* compute missing terms up to n */
    for (size_t i = used; i <= (size_t)n; ++i) {
        seq_t term =
            6 * cache[i - 1] - (seq_t)2 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term;
    }
    used = (size_t)n + 1;

    return cache[(size_t)n];
}
