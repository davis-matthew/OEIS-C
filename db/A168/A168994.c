/*
 * A168994  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff(n) + 27*Σ_{k=1}^{22} a(n‑k) + 378*a(n‑23) where
 *            coeff(n) = 1 if n==0 or n==23, = 2 if 1≤n≤22, = 0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;   /* array of computed terms     */
    static idx_t  cache_sz = 0;      /* size of current cache       */

    /* extend cache if required */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;
        cache = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        /* compute new terms in increasing order */
        for (idx_t i = cache_sz; i < new_sz; ++i) {
            seq_t term;
            /* starting contribution from p_n in the rational generating function */
            if (i == 0)            term = (seq_t)1;
            else if (i == 23)      term = (seq_t)1;
            else if (i <= 22)      term = (seq_t)2;
            else                   term = (seq_t)0;

            /* add 27 * a[i‑k] for k = 1 … 22 */
            for (int k = 1; k <= 22; ++k) {
                if (i >= (idx_t)k)
                    term += (seq_t)27 * cache[i - k];
            }
            /* add 378 * a[i‑23] if i ≥ 23 */
            if (i >= 23)
                term += (seq_t)378 * cache[i - 23];

            cache[i] = term;
        }
        cache_sz = new_sz;
    }
    return cache[n];
}
