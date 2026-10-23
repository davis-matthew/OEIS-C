/*
 * A169292  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq A169292: number of reduced words of length n in the Coxeter
   group on 39 generators with relations S_i^2 = (S_iS_j)^29 = I.
   Recurrence:
       a(0) = 1,
       a(n) = 2 + 37·∑_{k=0}^{n-1} a(k)                         (1 ≤ n < 29)
       a(n) = 37·∑_{k=1}^{28} a(n-k)  – 703·a(n-29)             (n ≥ 29)
   The first terms coincide with the OEIS data.
   The function keeps a cache so that repeated calls in any order
   are fast.  All arithmetic is performed with the native type seq_t.
*/

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of terms currently stored */

    /* Grow the cache if needed */
    if (cached_len <= n) {
        /* New length needed */
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        if (!new_cache) {
            /* Allocation failure: return 0 as a safe fallback. */
            return (seq_t)0;
        }

        /* Compute any missing terms */
        for (idx_t i = cached_len; i <= n; ++i) {
            seq_t val;

            if (i < 29) {
                /* Small n: use the initial recurrence */
                seq_t p = (i == 0) ? (seq_t)1 : (seq_t)2;
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j)
                    sum += new_cache[j];
                seq_t tmp = sum;
                tmp *= (seq_t)37;
                val = p + tmp;
            } else {
                /* Recurrence for n ≥ 29 */
                seq_t sum28 = 0;
                for (idx_t j = 1; j <= 28; ++j)
                    sum28 += new_cache[i - j];
                seq_t tmp1 = sum28;
                tmp1 *= (seq_t)37;
                seq_t tmp2 = new_cache[i - 29];
                tmp2 *= (seq_t)703;
                val = tmp1 - tmp2;
            }

            new_cache[i] = val;
        }

        cache = new_cache;
        cached_len = (idx_t)new_len;
    }

    return cache[n];
}
