/*
 * A123879  Expansion of (1-2*x+2*x^2-x^3)/(1-3*x+5*x^2-3*x^3+x^4).
 * https://oeis.org/A123879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 40, i64 n <= 80, i128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=0, a(3)=-3;
   a(n) = 3*a(n-1) - 5*a(n-2) + 3*a(n-3) - a(n-4) for n>=4 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements that are set */
    static size_t cache_cap = 0;   /* allocated size */

    /* grow cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = required;
        if (new_cap < 4) new_cap = 4;     /* at least first 4 entries */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first four terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)-3;
        cache_len = 4;
    }

    /* compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            cache[k] = 3 * cache[k-1] - 5 * cache[k-2] + 3 * cache[k-3] - cache[k-4];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
