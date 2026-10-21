/*
 * A232460  a(n) = 2^(2^n) - 5.
 * https://oeis.org/A232460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(2^n) - 5 Recurrence: a(0) = -3; a(n) = (a(n-1)+5)^2 -
 *            5 (n ≥ 1)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation of computed terms   */
    static size_t size  = 0;      /* current allocated size (0 ... size-1)   */
    static idx_t last   = 0;      /* highest index that has been computed   */

    /* initialise cache on the first call */
    if (cache == NULL) {
        size = (size_t)n + 1;                 /* allocate enough space   */
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!cache) exit(EXIT_FAILURE);       /* allocation failure      */
        cache[0] = (seq_t)-3;                 /* a(0) = -3               */
        last   = 0;                            /* last computed index     */
        if (n == 0) return cache[0];          /* quick return            */
    }

    /* grow cache if the requested index is beyond current allocation */
    if ((size_t)n + 1 > size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        size  = new_size;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t t    = prev + (seq_t)5;
        seq_t res  = t * t - (seq_t)5;
        cache[i]   = res;
    }

    last = n;            /* update the highest computed index */
    return cache[n];
}
