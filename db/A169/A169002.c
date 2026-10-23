/*
 * A169002  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169002
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

/* Sequence A169002: Number of reduced words of length n in a Coxeter group on 37 generators
   with relations Sᵢ² = (SᵢSⱼ)²³ = I.

   Recurrence:
     a(0) = 1
     for 1 ≤ n ≤ 22:
        a(n) = 2 + 35 * Σ_{k=1}^{min(22,n)} a(n-k)
     for n ≥ 23:
        a(n) = 35 * Σ_{k=1}^{22} a(n-k) – 630 * a(n-23)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   max_computed = 0;

    /* ensure the cache has space up to index n */
    if (cap <= n) {
        size_t newcap = cap ? cap << 1 : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* clear new entries */
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* base case */
    if (n == 0) {
        cache[0] = 1;
        max_computed = 0;
        return 1;
    }

    /* ensure a(0) is defined */
    if (cache[0] == 0) cache[0] = 1;

    /* if already computed, return */
    if (cache[n] != 0) return cache[n];

    /* compute values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 22) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 22 && k <= i; ++k) {
                sum += (seq_t)35 * cache[i - k];
            }
            cache[i] = (seq_t)2 + sum;            /* p_i = 2 for 1..22 */
        } else { /* i >= 23 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 22; ++k) {
                sum += (seq_t)35 * cache[i - k];
            }
            sum -= (seq_t)630 * cache[i - 23];
            cache[i] = sum;
        }
    }

    max_computed = n;
    return cache[n];
}
