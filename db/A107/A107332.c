/*
 * A107332  Expansion of x^2*(-1+x+x^2)/(-1+x+x^2-x^3+x^5).
 * https://oeis.org/A107332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-5) for n >= 5 * with
 *            initial values: a(1)=0, a(2)=1, a(3)=0, a(4)=0, a(5)=-1
 * Range:     i8 n <= 21, i16 n <= 36, i32 n <= 66, i64 n <= 126,
 *            i128 n <= 247, big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                   /* for size_t if needed */
#include "../utils/seq_malloc.h"     /* seq_malloc */
#include "../utils/seq_realloc.h"    /* seq_realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cached_max = 0;   /* highest index that is already initialized */

    /* If n is already cached, return it directly. */
    if (n <= cached_max && cache != NULL)
        return cache[n];

    /* Determine the size needed for the cache. */
    idx_t const new_max = n;   /* because cache indices start at 0 */
    seq_t *tmp;

    if (cache == NULL) {
        /* First allocation: use seq_malloc. */
        tmp = seq_malloc(new_max + 1);
    } else {
        /* Extend the existing array. */
        tmp = seq_realloc(cache, new_max + 1);
    }
    if (tmp == NULL)  /* allocation failed – return 0 as a safe fallback */
        return (seq_t)0;

    cache = tmp;

    /* Initialise the base cases up to new_max. */
    if (new_max >= 0) cache[0] = (seq_t)0;          /* unused, but convenient */
    if (new_max >= 1) cache[1] = (seq_t)0;
    if (new_max >= 2) cache[2] = (seq_t)1;
    if (new_max >= 3) cache[3] = (seq_t)0;
    if (new_max >= 4) cache[4] = (seq_t)0;
    if (new_max >= 5) cache[5] = (seq_t)-1;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cached_max + 1; i <= new_max; ++i) {
        if (i <= 5)            /* base values already set */
            continue;
        cache[i] = cache[i - 1] + cache[i - 2]
                 - cache[i - 3] + cache[i - 5];
    }

    cached_max = new_max;
    return cache[n];
}
