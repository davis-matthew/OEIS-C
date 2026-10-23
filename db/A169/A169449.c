/*
 * A169449  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169449.
   Recurrence:
     a(0) = 1
     a(n) = p_n + 2 * Σ_{k=1}^{32} a(n-k) – 3 * a(n-33)     (n ≥ 1)
   where p_n = 1 for n = 0 or 33; p_n = 2 for 1 ≤ n ≤ 32; p_n = 0 otherwise.
   The recurrence follows from the rational generating function
   (t³³ + 2t³² + … + 2t + 1) / (3t³³ – 2t³² – … – 2t – 1).
*/

#include <stddef.h>          /* for size_t */
#include <stdlib.h>          /* for NULL */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache to support out-of-order calls. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* cache contains terms 0 … cache_len-1 */

    /* Grow cache if necessary. */
    if (n >= cache_len) {
        idx_t   old_len = cache_len;
        size_t  new_len = (size_t)(n + 1);

        seq_t *new_cache = alloc_seq_array(new_len);
        if (!new_cache) {
            /* Allocation failed – return 0 as safe fallback. */
            return (seq_t)0;
        }

        /* Copy existing terms to the new buffer. */
        for (idx_t i = 0; i < old_len; ++i) {
            new_cache[i] = cache[i];
        }

        /* Free old buffer if it existed. */
        if (old_len > 0) {
            free_seq_array(cache);
        }

        cache      = new_cache;
        cache_len  = new_len;

        /* Compute missing terms from old_len up to n. */
        for (idx_t i = old_len; i <= n; ++i) {
            /* Sum of the previous up to 32 terms. */
            seq_t sum32 = 0;
            for (idx_t k = 1; k <= 32; ++k) {
                if (i >= k) {
                    sum32 += cache[i - k];
                }
            }

            /* Phot: p_n as defined above. */
            seq_t pn = 0;
            if (i == 0ULL)           pn = (seq_t)1;
            else if (i == 33ULL)     pn = (seq_t)1;
            else if (i <= 32ULL)     pn = (seq_t)2;

            /* Apply the recurrence. */
            seq_t term = pn + ( (seq_t)2 * sum32 );
            if (i >= 33ULL) {
                term -= ( (seq_t)3 * cache[i - 33] );
            }

            cache[i] = term;
        }
    }

    return cache[n];
}
