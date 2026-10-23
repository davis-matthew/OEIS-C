/*
 * A145978  Expansion of 1/(1-x*(1-8*x)).
 * https://oeis.org/A145978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 8*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 42, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last = 0;   /* highest index computed */
    static idx_t cap  = 0;   /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 2;                          /* need indices 0 and 1 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 1;
        last = 1;                          /* computed up to 1 */
    }

    if (n <= last) {
        return cache[n];
    }

    /* enlarge buffer if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute new terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) - 8*a(i-2) */
        cache[i] = cache[i - 1] - (seq_t)8 * cache[i - 2];
    }
    last = n;
    return cache[n];
}
