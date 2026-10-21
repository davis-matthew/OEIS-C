/*
 * A063307  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          34 ).
 * https://oeis.org/A063307
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13,
   offset 2.  Initial values for 2 <= n <= 13:
   a(2) = -1, a(3) = 11, a(4) = 24, a(5) = 36, a(6) = 48,
   a(7) = 60, a(8) = 72, a(9) = 82, a(10) = 96, a(11) = 106,
   a(12) = 120, a(13) = 132.  The function is callable in any order.
*/

#include <stdlib.h>

/* Initial values for indices 0..13.  Unused indices 0 and 1 are set to 0. */
static const seq_t init[14] = {
    0, 0,          /* a(0), a(1)  – not used */
   -1, 11, 24, 36, 48, 60, 72, 82, 96, 106, 120, 132
};

/* Cached sequence values; grows as needed. */
static seq_t *cache     = NULL;
static idx_t  cache_sz  = 0;      /* number of allocated entries */
static idx_t  last_computed = 1;  /* highest index that has been computed */

/* Helper: ensure the cache can hold indices up to 'need'. */
static int ensure_cache(idx_t need)
{
    /* Need at least need+1 entries (0..need).  Also ensure at least 14 */
    idx_t min_sz = need + 1;
    if (min_sz < 14)
        min_sz = 14;

    if (cache_sz >= min_sz)
        return 1;  /* already big enough */

    /* Grow size (double, but at least min_sz). */
    idx_t new_sz = cache_sz > 0 ? cache_sz * 2 : 32;
    if (new_sz < min_sz)
        new_sz = min_sz;

    /* realloc may fail. */
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp)
        return 0;  /* allocation failed */

    cache = tmp;
    cache_sz = new_sz;

    return 1;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* sequence undefined below offset */

    /* Allocate enough space. */
    if (!ensure_cache(n))
        return 0;  /* out of memory */

    /* Fill in any missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 13)                /* use initial table */
            cache[i] = init[i];
        else                        /* recurrence for n > 13 */
            cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
    }
    last_computed = n;

    return cache[n];
}
