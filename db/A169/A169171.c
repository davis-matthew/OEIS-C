/*
 * A169171  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function of A169171:
   Let b[0] = b[27] = 1, b[1..26] = 2, and b[n>27] = 0.
   For all n ≥ 0:
       a(n) = b[n] + 12 * Σ_{i=1}^{min(n,26)} a(n-i) – (n ≥ 27 ? 78 * a[n-27] : 0)
   This yields the sequence of reduced word counts for the stated Coxeter group. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Ensure storage for terms up to n */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t bi = 0;
        if (i == 0 || i == 27)
            bi = 1;
        else if (i <= 26)
            bi = 2;

        idx_t limit = (i < 27 ? i : 26);
        seq_t sum12 = 0;
        for (idx_t k = 1; k <= limit; ++k)
            sum12 += cache[i - k];

        seq_t val = bi + (seq_t)12 * sum12;
        if (i >= 27)
            val -= (seq_t)78 * cache[i - 27];

        cache[i] = val;
    }
    if (cache_size <= n)
        cache_size = n + 1;

    return cache[n];
}
