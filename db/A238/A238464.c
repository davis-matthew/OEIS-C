/*
 * A238464  Generalized ordered Bell numbers Bo(7,n).
 * https://oeis.org/A238464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * a(n) = 7*a(n-1) + 8*sum_{k odd} C(n-1,k)*a(n-k) -
 *            8*sum_{k even} C(n-1,k)*a(n-k) * a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of stored entries */
    size_t new_len;

    /* Expand the cache if the requested index is beyond what we have. */
    if ((size_t)n >= cache_len) {
        new_len = ((size_t)n) + 1;
        seq_t *new_cache = seq_alloc_new_array(new_len);

        /* Copy any existing entries. */
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) {
                new_cache[i] = cache[i];
            }
            free_seq_array(cache);
        }

        /* Compute all new entries up to n. */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {                 /* a(0) = 1 */
                new_cache[0] = (seq_t)1;
                continue;
            }
            if (i == 1) {                 /* a(1) = 7 */
                new_cache[1] = (seq_t)7;
                continue;
            }

            /* Even and odd parts of the alternating sum. */
            seq_t sum_even = (seq_t)0;
            seq_t sum_odd  = (seq_t)0;

            /* binomial coefficient C(i-1, k). */
            seq_t binom = (seq_t)1;   /* C(i-1, 0) */

            for (size_t k = 1; k < i; ++k) {
                /* Update binom to C(i-1, k). */
                binom = binom * (seq_t)(i - k) / (seq_t)k;

                seq_t term = binom * new_cache[i - k];
                if (k & 1) {                /* odd k */
                    sum_odd += term;
                } else {                    /* even k */
                    sum_even += term;
                }
            }

            /* a(i) = 7*a(i-1) + 8*sum_odd - 8*sum_even */
            seq_t temp = (seq_t)7 * new_cache[i - 1] + (seq_t)8 * sum_odd;
            new_cache[i] = temp - (seq_t)8 * sum_even;
        }

        cache      = new_cache;
        cache_len  = new_len;
    }

    return cache[(size_t)n];
}
