/*
 * A101916  G.f. satisfies: A(x) = 1/(1 + x*A(x^6)) and also the continued
 *          fraction: 1+x*A(x^7) =
 *          [1;1/x,1/x^6,1/x^36,1/x^216,...,1/x^(6^(n-1)),...].
 * https://oeis.org/A101916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -∑_{k=0..⌊(n‑1)/6⌋} a(k)·a(n‑6k‑1)
 * Range:     i8 n <= 34, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of computed terms */
static seq_t *cache      = NULL; /* array of terms */
static idx_t  cache_len  = 0;    /* allocated length of array   */
static idx_t  computed_max = 0;  /* highest index already computed */
static bool   initialized   = false; /* true after first term computed */

seq_t a(idx_t n)
{
    /* Resize the cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_buf = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_buf)
            return 0; /* allocation failure */

        /* Copy any previously computed values */
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i)
                new_buf[i] = cache[i];
            free(cache);
        }
        cache     = new_buf;
        cache_len = new_len;
    }

    /* Compute all missing terms up to n */
    idx_t start = initialized ? (computed_max + 1) : 0;
    for (idx_t i = start; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            seq_t sum = 0;
            idx_t max_k = (i - 1) / 6;
            for (idx_t k = 0; k <= max_k; ++k) {
                idx_t j = i - 6 * k - 1;
                sum += cache[k] * cache[j];
            }
            cache[i] = -sum;
        }
    }

    computed_max = n;
    initialized  = true;

    return cache[n];
}
