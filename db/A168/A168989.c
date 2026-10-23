/*
 * A168989  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of (t^23 + 2 t^22 + … + 2 t + 1) / (1 -
 *            22 t - 22 t^2 - … - 22 t^22 + 253 t^23) Hence a(0)=1, a(1)=24,
 *            a(2)=552,… and for n≥23 a(n) = 22 a(n−1)+…+22 a(n−22) – 253
 *            a(n−23)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic cache of pre‑computed values   */
    static size_t cache_len = 0;  /* number of valid entries in cache (max i+1) */
    static size_t cache_cap = 0;  /* allocated capacity                      */

    /* Extend cache capacity enough to hold values up to n (inclusive). */
    if ((idx_t)cache_len <= n) {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* abort on allocation failure   */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute values from the first missing index up to n.   */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t coeff;
            if (i == 0)           coeff = (seq_t)1;
            else if (i <= 22)     coeff = (seq_t)2;
            else if (i == 23)     coeff = (seq_t)1;
            else                  coeff = (seq_t)0;

            seq_t sum = (seq_t)0;
            /* Add 22 * a[i-k] for k = 1 … 22 (skip when i < k).   */
            for (int k = 1; k <= 22; ++k) {
                if (i >= (size_t)k) {
                    sum += (seq_t)22 * cache[i - k];
                }
            }
            /* For i ≥ 23 subtract 253 * a[i-23].                    */
            if (i >= 23) {
                sum -= (seq_t)253 * cache[i - 23];
            }

            cache[i] = coeff + sum;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
