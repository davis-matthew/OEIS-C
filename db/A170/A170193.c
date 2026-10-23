/*
 * A170193  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*Σ_{k=1}^{38} a(n-k) - 741*a(n-39) (n >= 40) * Initial
 *            values are computed from the generating function * (a(0)=1,
 *            a(1)=40, a(2)=1560, …). The recurrence is * derived from the
 *            rational generating function
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* next index that needs to be computed */

    /* compute initial 40 terms once */
    if (cache_len == 0) {
        size_t want = 40;
        if (cache_cap < want) {
            cache_cap = want;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        for (idx_t i = 0; i < 40; ++i) {
            seq_t rhs = (i == 0 || i == 39) ? 1 : 2;
            seq_t sumk = 0;
            for (idx_t k = 1; k <= 38 && i >= k; ++k)
                sumk += cache[i - k];
            seq_t val = rhs + 38 * sumk;
            if (i >= 39)   /* only i == 39 */
                val -= 741 * cache[i - 39];
            cache[i] = val;
        }
        cache_len = 40;
    }

    /* extend cache if needed */
    if ((idx_t)n >= cache_len) {
        idx_t need = n + 1;
        if (cache_cap < (size_t)need) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < (size_t)need) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sumk = 0;
            for (idx_t k = 1; k <= 38; ++k)
                sumk += cache[i - k];
            seq_t val = 38 * sumk - 741 * cache[i - 39];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
