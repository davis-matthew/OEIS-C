/*
 * A170428  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = N_n + 33*(a(n-1)+…+a(n-43)) – 561*a(n-44) where N_0 = 1,
 *            N_1..N_43 = 2, N_44 = 1, and N_n = 0 for n > 44
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size */
    static idx_t  cache_len = 0;      /* number of values stored (0..cache_len-1) */

    /* enlarge cache if necessary */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failed – return 0 as error */
        /* copy old values and zero the rest */
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t N = 0;
        if (i == 0)          N = (seq_t)1;
        else if (i <= 43)    N = (seq_t)2;
        else if (i == 44)    N = (seq_t)1;
        /* compute S = 33*(a(i-1)+…+a(i-43)) */
        seq_t sum43 = (seq_t)0;
        for (idx_t k = 1; k <= 43 && k <= i; ++k)
            sum43 += cache[i - k];
        seq_t term1 = (seq_t)33 * sum43;
        /* subtract T = 561*a(i-44) if i >= 44 */
        seq_t term44 = (seq_t)0;
        if (i >= 44)
            term44 = (seq_t)561 * cache[i - 44];
        cache[i] = N + term1 - term44;
    }
    cache_len = n + 1;
    return cache[n];
}
