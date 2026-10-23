/*
 * A160445  Numerator of Hermite(n, 20/21).
 * https://oeis.org/A160445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 40, * a(n) = 40*a(n-1) - 882*(n-1)*a(n-2) for n
 *            >= 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/* cached sequence storage ----------------------------------------*/
static seq_t *cache      = NULL;   /* array of computed terms   */
static size_t cap        = 0;      /* allocated size of cache   */
static size_t computed   = 0;      /* number of terms computed  */

/* Expand cache to hold at least n_plus_one elements */
static void ensure_cache(size_t n_plus_one)
{
    if (cap < n_plus_one)
    {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < n_plus_one) newcap <<= 1;

        /* allocate new array */
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */

        /* copy existing values if any */
        if (cache)
        {
            size_t j;
            for (j = 0; j < computed; ++j)
                tmp[j] = cache[j];
            free(cache);
        }

        cache = tmp;
        cap   = newcap;
    }
}

/* public function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise first two terms on first call */
    if (computed == 0)
    {
        ensure_cache(2);
        cache[0] = 1;
        cache[1] = 40;
        computed = 2;
    }

    /* expand cache and compute up to n if necessary */
    if ((size_t)n >= computed)
    {
        ensure_cache((size_t)n + 1);
        idx_t i;
        for (i = (idx_t)computed; i <= n; ++i)
        {
            seq_t t1 = 40 * cache[i - 1];
            seq_t t2 = 882 * ((seq_t)(i - 1)) * cache[i - 2];
            cache[i] = t1 - t2;
        }
        computed = (size_t)n + 1;
    }

    return cache[n];
}
