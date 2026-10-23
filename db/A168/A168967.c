/*
 * A168967  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = N_n + 48 * Σ_{k=1}^{min(n,21)} a(n-k) – 1176 * a(n-22),
 *            where N_n = 1 if n = 0 or n = 22, N_n = 2 if 1 ≤ n ≤ 21,
 *            otherwise 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cached values */
    static size_t cache_len = 0;       /* number of entries stored */

    /* enlarge cache if necessary */
    size_t need = (size_t)n + 1;
    if (cache_len < need) {
        size_t newlen = need > cache_len * 2 ? need : cache_len * 2;
        if (cache == NULL) {
            cache = (seq_t *)malloc(newlen * sizeof(seq_t));
            cache_len = 0;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
            cache = tmp;
        }
        /* compute missing terms */
        for (size_t i = cache_len; i < need; ++i) {
            seq_t term = 0;
            /* N_n */
            if (i == 0 || i == 22)
                term = 1;
            else if (i <= 21)
                term = 2;

            /* 48 × sum_{k=1}^{min(i,21)} a(i-k) */
            if (i > 0) {
                size_t up = i < 21 ? i : 21;
                seq_t sum = 0;
                for (size_t k = 1; k <= up; ++k)
                    sum += cache[i - k];
                term += (seq_t)((unsigned __int128)sum * (seq_t)48);
            }

            /* – 1176 × a(i-22) if applicable */
            if (i >= 22)
                term -= (seq_t)((unsigned __int128)cache[i - 22] * (seq_t)1176);

            cache[i] = term;
        }
        cache_len = need;
    }
    return cache[(size_t)n];
}
