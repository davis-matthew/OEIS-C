/*
 * A170234  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170234: Number of reduced words of length n in the Coxeter group
 * with generators S_i, (S_i)^2 = I and (S_i S_j)^40 = I.
 *
 * The generating function is (t^40+2 t^39+…+2 t+1) /
 * (496 t^40-31 t^39-…-31 t+1).  This yields a linear recurrence of
 * order 40:
 *
 *   a(0) = 1
 *   for 1 ≤ n ≤ 39:   a(n) = 2 + 31 * (a(n-1)+…+a(0))
 *   for n ≥ 40:       a(n) = 31 * (a(n-1)+…+a(n-39)) - 496 * a(n-40)
 *
 * The function below memoises the values in a static array so that
 * arbitrary calls out of order are handled efficiently.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for the sequence values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;

    /* Initialise cache with the first 40 terms */
    if (cache_len == 0) {
        /* Allocate space for indices 0..39 */
        cache_len = 40 + 1;
        cache     = (seq_t*)malloc(sizeof(seq_t) * cache_len);

        /* Compute a(0) */
        cache[0] = 1;

        /* Compute a(1) .. a(39) using the simplified recurrence
         * a(n) = 2 + 31 * sum_{k=0}^{n-1} a(k)
         */
        for (idx_t k = 1; k <= 39; ++k) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= k; ++j) {
                sum += cache[k - j];
            }
            cache[k] = (seq_t)2 + (seq_t)31 * sum;
        }
    }

    /* Extend cache if n is not yet computed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache          = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);

        /* Compute missing terms from the current size up to n */
        for (size_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            /* sum over the last 39 terms with coefficient 31 */
            for (idx_t j = 1; j <= 39; ++j) {
                sum += (seq_t)31 * cache[i - j];
            }
            /* Recurrence: a(i) = sum - 496 * a(i-40) */
            cache[i] = sum - (seq_t)496 * cache[i - 40];
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
