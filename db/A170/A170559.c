/*
 * A170559  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170559
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

/* OEIS A170559: Number of reduced words of length n in a Coxeter group of 22 generators
 * with relations S_i^2 = (S_i S_j)^47 = I.
 * Generating function:
 *     (t^47 + 2*t^46 + 2*t^45 + ... + 2*t + 1) /
 *     (1 - 20*t - 20*t^2 - ... - 20*t^46 + 210*t^47).
 * The coefficients satisfy the linear recurrence:
 *     a(n) = 20 * Σ_{k=1}^{46} a(n-k) - 210 * a(n-47) + h_n
 * where h_n = 1 for n = 0 or 47, h_n = 2 for 1 ≤ n ≤ 46, and h_n = 0 otherwise.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation – works for any call order */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* grow cache up to required index */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed – undefined behaviour */

        cache = new_cache;

        for (size_t i = cache_len; i < new_len; ++i) {
            /* sum of the previous 46 terms (with non‑negative indices) */
            seq_t sumPrev = (seq_t)0;
            for (size_t k = 1; k <= 46 && i >= k; ++k)
                sumPrev += cache[i - k];

            /* coefficient h_i from the numerator of the generating function */
            seq_t h;
            if (i == 0 || i == 47)
                h = (seq_t)1;
            else if (i < 48)
                h = (seq_t)2;
            else
                h = (seq_t)0;

            /* recurrence: a(i) = h + 20*sumPrev - 210*a(i-47)  (if i≥47) */
            seq_t term = h + ((seq_t)20) * sumPrev;
            if (i >= 47)
                term -= ((seq_t)210) * cache[i - 47];

            cache[i] = term;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
