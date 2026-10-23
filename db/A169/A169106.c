/*
 * A169106  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * Let a(0)=1. For n≥1 the numbers satisfy * a[n] =
 *            43·∑_{k=1}^{min(24,n)} a[n‑k] – 946·a[n‑25] + b[n] * where *
 *            b[n] = 2 for 1 ≤ n ≤ 24 * b[25] = 1 * b[n] = 0 otherwise * *
 *            This recurrence follows from the rational generating function *
 *            A(t) = (t^25 + 2t^24 + … + 2t + 1)/(1 – 43t – … – 43t^24 + 9...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms a[0..current_max] */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t  current_max = 0;

    /* Initialise cache with a[0] = 1 */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
        current_max = 0;
    }

    /* Return the value immediately if it has already been computed */
    if (n <= current_max) {
        return cache[n];
    }

    /* Resize the cache if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache,
                                      new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms current_max+1 .. n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t sum24 = 0;

        /* Sum the last up to 24 terms a[i‑k] */
        for (idx_t k = 1; k <= 24 && i >= k; ++k) {
            sum24 += cache[i - k];
        }

        seq_t val = (seq_t)43 * sum24;     /* 43 · (sum of past 24) */

        /* Subtract 946·a[i‑25] if i ≥ 25 */
        if (i >= 25) {
            val -= (seq_t)946 * cache[i - 25];
        }

        /* Add the numerator coefficient b[i] */
        if (i <= 24) {
            val += (seq_t)2;
        } else if (i == 25) {
            val += (seq_t)1;
        }
        /* For i > 25, b[i] = 0, nothing to add */

        cache[i] = val;
    }

    current_max = n;
    return cache[n];
}
