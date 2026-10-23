/*
 * A169290  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169290
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

/* Recurrence: a(n) = N_n + 35*sum_{k=1}^{28} a(n-k) - 630*a(n-29)
   where
       N_0 = N_29 = 1,
       N_k = 2 for 1 ≤ k ≤ 28,
       N_k = 0 otherwise
   (indices n ≥ 0); a(n) = 0 for n < 0. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* current capacity of the cache */
    static idx_t size = 0;  /* number of already computed terms */

    /* initialise cache with a(0) */
    if (size == 0) {
        size = 1;
        cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory: return 0 */
        cache[0] = (seq_t)1;           /* a(0) = 1 */
    }

    /* grow cache if needed */
    if (n >= size) {
        idx_t new_cap = cap == 0 ? 64 : cap;
        while (new_cap <= n) new_cap <<= 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            /* sum of the previous up to 28 terms */
            for (int k = 1; k <= 28; ++k) {
                if (i >= (idx_t)k)
                    sum += cache[i - k];
            }

            /* N_i contribution */
            seq_t N;
            if (i == 0 || i == 29)
                N = (seq_t)1;
            else if (i <= 28)
                N = (seq_t)2;
            else
                N = (seq_t)0;

            /* recurrence: a(i) = N + 35*sum - 630*a(i-29) */
            seq_t term = (i >= 29) ? cache[i - 29] * (seq_t)630 : (seq_t)0;
            cache[i] = N + sum * (seq_t)35 - term;
        }
        size = n + 1;
    }

    return cache[n];
}
