/*
 * A099530  Expansion of 1/(1 - x + x^4).
 * https://oeis.org/A099530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - a(n-4) with initial terms a(0) = 1, a(1) = 1,
 *            a(2) = 1, a(3) = 1, a(4) = 0
 * Range:     i8 n <= 29, i16 n <= 64, i32 n <= 129, i64 n <= 259,
 *            i128 n <= 523, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of terms */
    static idx_t  len   = 0;      /* number of terms stored */
    static idx_t  cap   = 0;      /* allocated capacity */

    /* initialise the cache on first call */
    if (cache == NULL) {
        /* start with enough room for the first few terms */
        const idx_t init_cap = 8;
        cache = (seq_t *)malloc((size_t)init_cap * sizeof(seq_t));
        if (!cache)
            return 0;   /* allocation failure */

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 0;
        len = 5;
        cap = init_cap;
    }

    /* if the requested term is already cached, return it */
    if (n < len)
        return cache[n];

    /* enlarge the cache if necessary */
    if (n >= cap) {
        /* new capacity at least n+1 */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;   /* reallocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        /* i >= 5 for i-4 to be valid */
        cache[i] = cache[i - 1] - cache[i - 4];
    }
    len = n + 1;

    return cache[n];
}
