/*
 * A015271  Gaussian binomial coefficient [ n,3 ] for q = -4.
 * https://oeis.org/A015271
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -51*a(n-1)+884*a(n-2)+3264*a(n-3)-4096*a(n-4) (n>=7) *
 *            initial values: a(3)=1, a(4)=-51, a(5)=3485, a(6)=-219555
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; behaviour is unspecified for n<3 */
    if (n < 3) return (seq_t)0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* Grow cache if needed */
    if (n > max_cached) {
        /* Allocate or reallocate cache */
        size_t new_size = (size_t)(n + 1);              /* array size from 0 .. n */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }

        /* Initialize newly allocated part to 0 (unnecessary but safe) */
        for (size_t i = max_cached + 1; i < new_size; ++i)
            cache[i] = (seq_t)0;

        /* Compute terms up from max_cached+1 to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i < 3) continue; /* values below offset are irrelevant */

            if (i == 3) cache[i] = (seq_t)1;
            else if (i == 4) cache[i] = (seq_t)-51;
            else if (i == 5) cache[i] = (seq_t)3485;
            else if (i == 6) cache[i] = (seq_t)-219555;
            else
                cache[i] =
                    (seq_t)(-51) * cache[i - 1] +
                    (seq_t)884 * cache[i - 2] +
                    (seq_t)3264 * cache[i - 3] -
                    (seq_t)4096 * cache[i - 4];
        }

        /* Update maximum cached index */
        max_cached = n;
    }

    return cache[n];
}
