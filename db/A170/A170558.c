/*
 * A170558  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170558: Number of reduced words of length n in a Coxeter group on
 * 21 generators with relations (S_i)^2 = (S_i S_j)^47 = I.
 *
 * The sequence satisfies the following recurrence:
 *     a(0) = 1
 *     a(n) = 2 + 19 * Σ_{k=0}^{n-1} a(k)                          for 1 ≤ n ≤ 46
 *     a(47) = 19 * Σ_{k=1}^{46} a(k) – 208
 *     a(n) = 19 * Σ_{k=1}^{46} a(n-k) – 190 * a(n-47)              for n ≥ 48
 *
 * The function below memoizes computed values in a static array so that
 * calls may appear in any order.  All arithmetic is performed using the
 * generic integer type seq_t, and indices use idx_t.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries currently stored */

    /* Allocate or grow the cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure – caller should handle */
        cache = tmp;
        cache_size = new_size;

        /* Compute terms that are not yet present */
        idx_t i;
        for (i = cache_size - (n + 1); i <= n; ++i) {   /* start at old size */
            /* For i==0 the base case */
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            /* Compute cumulative sums that may be needed later */
            if (i <= 46) {
                /* a(i) = 2 + 19 * Σ_{k=0}^{i-1} a(k) */
                seq_t sum_prev = 0;
                idx_t j;
                for (j = 0; j < i; ++j) sum_prev += cache[j];
                cache[i] = (seq_t)2 + (seq_t)19 * sum_prev;
                continue;
            }

            if (i == 47) {
                /* a(47) = 19 * Σ_{k=1}^{46} a(k) – 208 */
                seq_t sum_1_to_46 = 0;
                idx_t j;
                for (j = 1; j <= 46; ++j) sum_1_to_46 += cache[j];
                cache[47] = (seq_t)19 * sum_1_to_46 - (seq_t)208;
                continue;
            }

            if (i >= 48) {
                /* Sliding window sum of the previous 46 terms
                 * for the recurrence a(i) = 19 * Σ_{k=1}^{46} a(i-k) – 190 * a(i-47)
                 */
                seq_t sum_last46 = 0;
                idx_t j;
                /* Initialise the window that ends at i-1:
                 * indices from (i-46) to (i-1)
                 */
                for (j = i - 46; j <= i - 1; ++j)
                    sum_last46 += cache[j];
                cache[i] = (seq_t)19 * sum_last46 - (seq_t)190 * cache[i - 47];
            }
        }
    }

    return cache[n];
}
