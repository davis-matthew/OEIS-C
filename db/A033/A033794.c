/*
 * A033794  Product t2(q^d); d | 35, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|35} ∑_{k≥0} q^{d·k(k+1)/2}
 *            (product over divisors d = 1,5,7,35 of the series t2(q^d))
 * Range:     u8 n <= 1316, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* pre‑computed values */
static idx_t cache_len = 0;    /* number of entries in cache */

seq_t a(idx_t n)
{
    /* Allocate or grow the cache to hold values up to n. */
    if (n + 1 > cache_len) {
        idx_t len  = n + 1;
        idx_t dVal[] = {1,5,7,35};
        const size_t nd = sizeof(dVal)/sizeof(dVal[0]);

        /* Temporary arrays for convolution */
        seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
        seq_t *cur  = (seq_t *)calloc(len, sizeof(seq_t));
        if (!prev || !cur) { /* allocation failure – return 0 */
            free(prev);
            free(cur);
            return (seq_t)0;
        }
        prev[0] = (seq_t)1;   /* start with 1 */

        for (size_t di = 0; di < nd; ++di) {
            idx_t d = dVal[di];
            /* clear cur */
            for (idx_t i = 0; i <= n; ++i) cur[i] = (seq_t)0;

            /* convolution with the series for divisor d */
            for (idx_t k = 0;; ++k) {
                idx_t v = d * k * (k + 1) / 2;
                if (v > n) break;
                for (idx_t i = v; i <= n; ++i)
                    cur[i] += prev[i - v];
            }

            /* swap prev and cur for next iteration */
            seq_t *tmp = prev; prev = cur; cur = tmp;
        }

        /* copy results into cache */
        seq_t *new_cache = (seq_t *)realloc(cache, len * sizeof(seq_t));
        if (!new_cache) {
            free(prev);   /* prev is the final array (i.e., cur after last swap) */
            free(cur);    /* cur is the temporary one */
            return (seq_t)0;
        }
        cache = new_cache;
        for (idx_t i = 0; i < len; ++i) cache[i] = prev[i];
        cache_len = len;

        /* free temporary arrays (cur holds the pre‑final array) */
        free(prev); /* prev now points to the final result array */
        free(cur);
    }

    return cache[n];
}
