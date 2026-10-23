/*
 * A160583  Positive numbers y such that y^2 is of the form x^2+(x+521)^2 with
 *          integer x.
 * https://oeis.org/A160583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3)-a(n-6) for n > 6; a(1)=445, a(2)=521, a(3)=629,
 *            a(4)=2041, a(5)=2605, a(6)=3329
 * Range:     u8 none, u16 n <= 9, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*   cache keeps all values up to the largest n that has been computed 
          so far.  The cache is grown lazily to keep memory usage low. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1, return 0 */

    if (n > cache_size) {
        /* Grow the cache array to hold indices 1..n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp)                         /* allocation failed – unlikely */
            return (seq_t)0;
        cache = tmp;

        /* Initialise the first six terms (if needed) */
        static const seq_t init[6] = {445, 521, 629, 2041, 2605, 3329};

        /* If no terms were stored before, set all up to n (but not beyond 6) */
        if (cache_size == 0) {
            for (idx_t i = 1; i <= n && i <= 6; ++i)
                cache[i - 1] = init[i - 1];
        } else if (cache_size < 6) {
            /* We had some terms previously, but fewer than six;
               fill the missing initial terms as required. */
            for (idx_t i = cache_size + 1; i <= n && i <= 6; ++i)
                cache[i - 1] = init[i - 1];
        }

        /* Compute any remaining terms using the recurrence.
           Terms 1..6 are already in the cache.  We start from the
           smallest index that needs the recurrence, which is 7. */
        idx_t start = cache_size + 1;
        if (start < 7) start = 7;
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = 6*a(i-3) - a(i-6) */
            cache[i - 1] = 6 * cache[i - 4] - cache[i - 7];
        }
        cache_size = n;
    }

    /* Cache now contains a(n) */
    return cache[n - 1];
}
