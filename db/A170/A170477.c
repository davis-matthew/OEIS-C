/*
 * A170477  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 44: a(n) = 2 + 34 * Σ_{k=0}^{n-1} a(k) For
 *            n ≥ 45: a(n) = 34 * Σ_{k=n-44}^{n-1} a(k) – 595 * a(n-45)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;   /* array of a(i) */
    static seq_t *prefix = NULL;  /* prefix sums: prefix[i] = Σ_{k=0}^{i} a(k) */
    static idx_t cache_len = 0;   /* number of currently cached elements */

    /* check whether we need to extend the cache */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;

        /* allocate new arrays of the required size */
        seq_t *new_cache  = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *new_prefix = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache || !new_prefix) {
            /* allocation failure – return 0 (caller must handle) */
            return (seq_t)0;
        }

        /* copy existing values if any */
        for (idx_t i = 0; i < old_len; ++i) {
            new_cache[i]  = cache[i];
            new_prefix[i] = prefix[i];
        }
        /* free old memory */
        free(cache);
        free(prefix);

        /* switch to the new arrays */
        cache   = new_cache;
        prefix  = new_prefix;
        cache_len = new_len;

        /* start computing from where we left off */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t a_val;
            if (i == 0) {
                a_val = (seq_t)1;
            } else if (i < 45) {
                seq_t sum_all = prefix[i - 1];
                a_val = sum_all * (seq_t)34;
                a_val += (seq_t)2;
            } else {
                seq_t sum_last44 = prefix[i - 1] - prefix[i - 45];
                a_val = sum_last44 * (seq_t)34;
                a_val -= cache[i - 45] * (seq_t)595;
            }
            cache[i] = a_val;
            prefix[i] = a_val + ((i == 0) ? (seq_t)0 : prefix[i - 1]);
        }
    }

    return cache[n];
}
