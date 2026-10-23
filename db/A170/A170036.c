/*
 * A170036  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies the linear recurrence with * order 36: *
 *            * a(0) = 1, * a(n) = 1 + 25 * Σ_{k=0}^{n-1} a(k) for 1 ≤ n ≤ 35
 *            (coefficients of the numerator are 1 for n=0, 2 for n=1…35) *
 *            a(36) = 25 * Σ_{k=1}^{35} a(k) - 324 * a(n) = 25 * Σ_{k=1}^{35}
 *            a(n-k) - 325 * a(n-36) for n ≥ 37 * * The g...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* ensure enough space in the cache */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1u : 64u;
        while (new_cap <= (size_t)n) new_cap <<= 1u;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise the cache up to a(36) once */
    if (cache_len == 0) {
        seq_t sum = 0;                 /* Σ_{k=0}^{m-1} a(k) for current m */
        for (idx_t m = 0; m < 36; ++m) {
            seq_t n_term = (m == 0) ? (seq_t)1 : (seq_t)2;
            seq_t val    = n_term + (seq_t)25 * sum;
            cache[m]     = val;
            sum += val;
        }
        /* a(36) = 25 * Σ_{k=1}^{35} a(k) - 324
         * Σ_{k=1}^{35} a(k) = sum - a(0)
         */
        seq_t S35 = sum - cache[0];
        cache[36] = (seq_t)25 * S35 - (seq_t)324;
        cache_len = 37;             /* indices 0 … 36 are now ready */
    }

    /* compute missing values up to n */
    if ((size_t)n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* only n ≥ 37 enters this loop
             * a(i) = 25 * Σ_{k= i-35}^{i-1} a(k) - 325 * a(i-36)
             */
            seq_t sum35 = 0;
            for (idx_t j = i - 35; j < i; ++j)
                sum35 += cache[j];
            seq_t val = (seq_t)25 * sum35 - (seq_t)325 * cache[i - 36];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
