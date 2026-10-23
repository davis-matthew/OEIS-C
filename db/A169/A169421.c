/*
 * A169421  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169421
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

/* Recurrence:
   a(0) = 1
   for 1 ≤ n ≤ 31:   a(n) = 2 + 22 * Σ_{k=1}^{n}     a(n-k)
   for n = 32:        a(32) = 1 + 22 * Σ_{k=1}^{31}  a(32-k) - 253 * a(0)
   for n ≥ 33:        a(n) = 22 * Σ_{k=1}^{31}  a(n-k) - 253 * a(n-32)
   (Σ denotes a sum of 31 preceding terms.)
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated size */
    static idx_t  computed = 0;    /* number of elements already computed */

    /* Ensure the cache is large enough */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure, caller should handle */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            /* number of terms to sum (≤31) */
            int limit = (i < 31) ? (int)i : 31;
            for (int k = 1; k <= limit; ++k) {
                /* i - k is always ≤ i, so no underflow */
                sum += cache[(idx_t)(i - k)];
            }

            seq_t val;
            if (i <= 31) {
                /* 2 + 22*sum of all previous terms */
                val = (seq_t)2 + (seq_t)22 * sum;
            } else if (i == 32) {
                /* 1 + 22*sum_{k=1}^{31}a(32-k) - 253*a(0) */
                val = (seq_t)1 + (seq_t)22 * sum - (seq_t)253 * cache[0];
            } else {
                /* 22*sum_{k=1}^{31}a(n-k) - 253*a(n-32) */
                val = (seq_t)22 * sum - (seq_t)253 * cache[i - 32];
            }
            cache[i] = val;
        }
    }
    computed = n + 1;
    return cache[n];
}
