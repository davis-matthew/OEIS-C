/*
 * A160316  Numerator of Hermite(n, 18/31).
 * https://oeis.org/A160316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160316: Numerator of Hermite(n,18/31)
   a(0) = 1
   a(1) = 36
   Recurrence for n ≥ 2:
     a(n) = 36*a(n‑1) – 1922*(n‑1)*a(n‑2)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of computed terms   */
    static size_t cache_size = 0;    /* number of filled cache entries */

    /* Expand the cache if necessary */
    if (cache_size <= n)
    {
        size_t new_size = n + 1;

        if (cache == NULL)         /* first allocation */
        {
            cache = (seq_t *)calloc(new_size, sizeof(seq_t));
            if (cache == NULL)
                return 0;          /* allocation failure – caller will not expect this */

            /* initial terms */
            cache[0] = 1;
            if (new_size > 1) cache[1] = 36;

            /* compute any remaining terms */
            for (idx_t i = 2; i <= n; ++i)
            {
                seq_t term1 = cache[i - 1] * 36;
                seq_t mul    = (seq_t)(i - 1) * 1922;
                seq_t term2  = cache[i - 2] * mul;
                cache[i]    = term1 - term2;
            }
            cache_size = new_size;
        }
        else                       /* enlarging an existing cache */
        {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL)
                return 0;          /* allocation failure */
            cache = tmp;

            /* If the cache previously held only a(0), initialize a(1) */
            if (new_size > 1 && cache_size < 2)
                cache[1] = 36;

            /* compute new terms */
            for (idx_t i = cache_size; i <= n; ++i)
            {
                seq_t term1 = cache[i - 1] * 36;
                seq_t mul    = (seq_t)(i - 1) * 1922;
                seq_t term2  = cache[i - 2] * mul;
                cache[i]    = term1 - term2;
            }
            cache_size = new_size;
        }
    }

    return cache[n];
}
