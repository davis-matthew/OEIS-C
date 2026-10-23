/*
 * A109265  Row sums of Riordan array (1-x-x^2,x(1-x)).
 * https://oeis.org/A109265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=-2, a(3)=-2, a(n) = -a(n-3) for n ≥ 4
 *            (row‑sum recurrence)
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* static storage for computed values       */
static idx_t cache_size = 0;  /* number of valid elements in cache       */

seq_t a(idx_t n)
{
    /* initialise cache on first call                                 */
    if (cache == NULL)
    {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            exit(EXIT_FAILURE);                /* out of memory          */

        cache[0] = (seq_t)  1;
        cache[1] = (seq_t)  0;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t)-2;
    }

    /* already computed?  */
    if (n < cache_size)
        return cache[n];

    /* extend cache to include n                                 */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL)
        exit(EXIT_FAILURE);                /* out of memory          */
    cache = tmp;

    /* compute missing values using the recurrence                  */
    for (idx_t i = cache_size; i <= n; ++i)
        cache[i] = -cache[i - 3];

    cache_size = n + 1;
    return cache[n];
}
