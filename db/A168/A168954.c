/*
 * A168954  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 35*∑_{k=1}^{21}a(n‑k) – 630*a(n‑22), * where
 *            p(0)=1, p(1…21)=2, p(22)=1, p(n>22)=0. * This recurrence follows
 *            from the rational generating function * of A168954
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;      /* number of valid entries */

    /* grow cache to accommodate n */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 as a sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if needed */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* p(i) */
        seq_t pi;
        if (i == 0)
            pi = (seq_t)1;
        else if (i <= 21)
            pi = (seq_t)2;
        else if (i == 22)
            pi = (seq_t)1;
        else
            pi = (seq_t)0;

        /* sum of last up to 21 terms */
        seq_t sum21 = 0;
        for (size_t k = 1; k <= 21; ++k) {
            if (i >= k)
                sum21 += cache[i - k];
        }

        seq_t term = pi + (seq_t)35 * sum21;

        if (i >= 22) {
            term -= (seq_t)630 * cache[i - 22];
        }

        cache[i] = term;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
