/*
 * A170551  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170551
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

/* Computed by linear recurrence derived from the rational generating function;
   Initial values: a(0) = 1, a(1) = 14, a(n) = 14 * 13^(n-1) for 2 ≤ n ≤ 46.
   For n ≥ 47: a(n) = 12 * Σ_{i=1}^{46} a(n-i) – 78 * a(n-47). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of computed entries */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (cache) {
            for (size_t i = 0; i < (size_t)cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)14;
        } else if (i <= 46) {
            cache[i] = cache[i-1] * ((seq_t)13);
        } else { /* i >= 47 */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 46; ++j)
                sum += cache[i-j];
            cache[i] = sum * ((seq_t)12) - (seq_t)78 * cache[i-47];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
