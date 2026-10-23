/*
 * A174756  x-values in the solution to x^2-51*y^2=1.
 * https://oeis.org/A174756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1)-a(n-2) with a(1)=1, a(2)=50
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th element of OEIS A174756 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t  *cache   = NULL;   /* array of computed values */
    static size_t  cap     = 0;      /* allocated capacity (in items) */
    static size_t  len     = 0;      /* number of valid entries (max index stored) */

    /* Ensure the cache is large enough for n elements */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 1;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return zero as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing elements up to n */
    if (len < (size_t)n) {
        for (idx_t i = len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
            } else if (i == 2) {
                cache[1] = (seq_t)50;
            } else {
                /* Use the linear recurrence a(n) = 100*a(n-1)-a(n-2) */
                seq_t prev  = cache[i - 2];       /* a(i-1) */
                seq_t prev2 = cache[i - 3];       /* a(i-2) */
                cache[i - 1] = (seq_t)100 * prev - prev2;
            }
        }
        len = (size_t)n;
    }

    return cache[n - 1];
}
