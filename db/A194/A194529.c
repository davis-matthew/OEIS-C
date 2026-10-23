/*
 * A194529  Second coordinate of (5,8)-Lagrange pair for n.
 * https://oeis.org/A194529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194529
   a(1..14) = {  2, -1,  1, -2,  0,  2, -1,  1,  3,  0,  2,  4,  1,  3 }
   for n>14: a(n) = a(n-1) + a(n-13) - a(n-14)              (OEIS formula) */

#include <stdlib.h>   /* malloc/realloc/abort */

static const seq_t init[14] = { 2, -1, 1, -2, 0, 2, -1, 1, 3, 0, 2, 4, 1, 3 };
static seq_t *cache   = NULL;    /* cache[0] = a(1), cache[cache_len-1] = a(cache_len) */
static idx_t cache_len = 0;       /* number of values currently stored */

/*---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* For the first 14 terms we know the values a priori.  No allocation is
       required for these, but we still honour any later requests for them.   */
    if (n <= 14)
        return init[n - 1];

    /* Grow the cache if it does not yet contain the desired term.               */
    if (cache_len < n) {
        size_t new_len = (size_t)n;                 /* n is small enough for size_t */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                         /* out of memory */

        /* Compute every missing term up to n.  We reuse existing values
           when they are already in the cache.                                */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 14)
                tmp[i - 1] = init[i - 1];
            else
                tmp[i - 1] = tmp[i - 2] + tmp[i - 14] - tmp[i - 15];
        }

        cache = tmp;
        cache_len = n;
    }

    return cache[n - 1];
}
