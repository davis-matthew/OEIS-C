/*
 * A169007  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from the generating
 *            function G(t) = (t^23 + 2t^22 + … + 2t + 1) / ( 1 – 40t – 40t^2
 *            – … – 40t^22 + 820t^23 ). Let g(n) be the numerator coefficient:
 *            g(0) = 1, g(1…22) = 2, g(23) = 1, g(n)=0 for n>23. For n ≥ 0
 *            a(n) = g(n) + 40·(a(n-1)+a(n-2)+…+a(n-22))...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of computed values */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of cached entries */

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Expand cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t old_size = cache_size;
        size_t newsize  = (size_t)n + 1;

        /* allocate new cache array */
        seq_t *newcache = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (!newcache) return 0;               /* allocation failed */

        /* copy already computed values */
        for (size_t i = 0; i < old_size; ++i)
            newcache[i] = cache[i];

        /* free old cache and update pointers */
        if (cache) free(cache);
        cache = newcache;
        cache_size = newsize;

        /* compute values that have not yet been cached */
        for (idx_t i = (idx_t)old_size; i <= n; ++i) {
            /* numerator coefficient g(i) */
            seq_t g;
            if (i == 0 || i == 23)
                g = 1;
            else if (i <= 22)
                g = 2;
            else
                g = 0;

            /* sum of the last up to 22 terms */
            seq_t sum = 0;
            idx_t kmax = (i < 22) ? i : 22;
            for (idx_t k = 1; k <= kmax; ++k)
                sum += cache[i - k];

            /* apply the recurrence */
            seq_t val = g + (seq_t)40 * sum;
            if (i >= 23)
                val -= (seq_t)820 * cache[i - 23];

            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}
