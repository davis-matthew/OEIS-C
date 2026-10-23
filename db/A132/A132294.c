/*
 * A132294  The first three terms are 1. After that,
 *          a(n)=(a(n-1))^2-a(n-2)-a(n-3).
 * https://oeis.org/A132294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1)^2 - a(n-2) - a(n-3), with a(1)=a(2)=a(3)=1
 * Range:     i8 n <= 9, i16 n <= 10, i32 n <= 11, i64 n <= 12, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;          /* pointer to the cache array  */
    static idx_t cache_cap = 0;          /* current capacity of the cache */
    static idx_t computed = 0;           /* highest index that has been computed */

    if (n < 1)           /* behaviour for indices below the offset is undefined */
        return 0;

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first three terms (only if the array supports them) */
    if (cache_cap > 1) cache[1] = 1;
    if (cache_cap > 2) cache[2] = 1;
    if (cache_cap > 3) cache[3] = 1;

    if (n <= 3)
        return 1;            /* a(1)=a(2)=a(3)=1 */

    /* ensure that we have computed the first three terms */
    if (computed < 3)
        computed = 3;

    /* compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * cache[i-1] - cache[i-2] - cache[i-3];
    }
    computed = n;
    return cache[n];
}
