/*
 * A179944  Row sums of triangle A179943.
 * https://oeis.org/A179944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∑_{k≥0} x^k/(1-(k+2)*x+x^2) (row
 *            sums of triangle A179943)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* The function a(idx_t n) must be defined with external linkage. */
seq_t a(idx_t n)
{
    /* static buffer to memoize computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;         /* number of elements stored (n+1) */

    /* extend cache if needed */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory */
        memset(new_cache, 0, new_size * sizeof(seq_t));

        /* compute values up to n */
        for (idx_t k = 0; k <= n; ++k) {
            seq_t base = (seq_t)(k + 2);   /* (k+2) as seq_t */

            /* m = 0 */
            new_cache[k] += (seq_t)1;

            if (n - k >= 1) {
                /* m = 1 */
                new_cache[k + 1] += base;

                seq_t prev2 = (seq_t)1;     /* c0 */
                seq_t prev1 = base;        /* c1 */

                /* m >= 2 */
                for (idx_t m = 2; m <= n - k; ++m) {
                    seq_t cur = base * prev1 - prev2;
                    new_cache[k + m] += cur;
                    prev2 = prev1;
                    prev1 = cur;
                }
            }
        }

        /* replace old cache */
        if (cache) free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
