/*
 * A168958  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 41*40^(n-1) for 1 ≤ n ≤ 21, a(0) = 1, and for n ≥ 22 the
 *            recurrence a(n) = 39*a(n-1)+39*a(n-2)+⋯+39*a(n-21)-780*a(n-22)
 *            holds
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t  cache_len  = 0;   /* actual number of terms stored */

    /* ensure at least n+1 elements can be stored */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed: caller is responsible for error handling */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i < 22) {
            /* a(i) = 41 * 40^(i-1) */
            seq_t p = (seq_t)1;
            for (idx_t k = 1; k < i; ++k)
                p = p * (seq_t)40;
            val = (seq_t)41 * p;
        } else {
            /* a(i) = 39*a(i-1)+…+39*a(i-21)-780*a(i-22) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 21; ++k)
                sum += (seq_t)39 * cache[i - k];
            sum -= (seq_t)780 * cache[i - 22];
            val = sum;
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
