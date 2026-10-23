/*
 * A168886  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*Σ_{i=1}^{20} a(n-i) – 120*a(n-21) + p(n) p(0)=1,
 *            p(1…20)=2, p(21)=1, p(n)=0 for n>21 (this is the recurrence
 *            derived from the rational generating function)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values currently stored */
    static idx_t cache_cap   = 0;   /* capacity of the cache array */

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* out‑of‑memory sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n, if not already done */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* sum of the last 20 terms a(i-1) … a(i-20) */
        seq_t sum20 = 0;
        for (idx_t k = 1; k <= 20 && i >= k; ++k) {
            sum20 += cache[i - k];
        }

        /* p(i) as defined above */
        seq_t pi = 0;
        if (i == 0) {
            pi = (seq_t)1;
        } else if (i <= 20) {
            pi = (seq_t)2;
        } else if (i == 21) {
            pi = (seq_t)1;
        }   /* otherwise pi stays 0 */

        seq_t val;
        if (i == 0) {
            val = pi;                /* 1 */
        } else {
            seq_t term1 = (seq_t)15 * sum20;
            seq_t term2 = (i >= 21) ? (seq_t)120 * cache[i - 21] : (seq_t)0;
            val = term1 - term2 + pi;
        }
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
