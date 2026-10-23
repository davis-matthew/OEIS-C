/*
 * A169225  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 27 : a(n) = 2 + 18 * Σ_{k=1}^{n} a(n‑k) For
 *            n = 28 : a(28) = 1 + 18 * Σ_{k=1}^{27} a(28‑k) - 171 * a(0) For
 *            n ≥ 29 : a(n) = 18 * Σ_{k=1}^{27} a(n‑k) - 171 * a(n‑28) or
 *            equivalently a(n) = b(n) + 18 * Σ_{k=1}^{min(n,27)} a(n‑k) –
 *            (n≥28? 171*a(n‑28) : 0) where b(0)=1, b(1…27)=2...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;

    if (n < cache_size)
        return cache[n];

    /* ensure capacity */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* out of memory; caller may handle this */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* compute new terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        idx_t k_lim = (i <= 27 ? i : 27);
        seq_t sum = 0;
        for (idx_t k = 1; k <= k_lim; ++k)
            sum += cache[i - k];

        seq_t val = (i <= 27 ? (seq_t)2
                     : (i == 28 ? (seq_t)1 : (seq_t)0)) + ((seq_t)18 * sum);

        if (i >= 28)
            val -= (seq_t)171 * cache[i - 28];

        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
