/*
 * A166172  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*(a[n-1]+a[n-2]+...+a[n-9]) - 741*a[n-10] + c[n], where
 *            c[0] = 1, c[1..9] = 2, c[10] = 1, c[n] = 0 for n > 10 (the
 *            recurrence derived from the generating function)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static const int coeffA[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};

    size_t need = (size_t)n + 1;
    if (need > cache_len) {
        /* grow the cache */
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        /* initial allocation: if cache was NULL, new_cache is same as cache */
        cache = new_cache;
        /* compute new terms */
        for (size_t i = cache_len; i < need; ++i) {
            seq_t sum = 0;
            /* sum of up to the previous 9 terms */
            for (int j = 1; j <= 9 && i >= (size_t)j; ++j) {
                sum += cache[i - j];
            }
            seq_t term = sum * ((seq_t)38);
            seq_t sub = 0;
            if (i >= 10) {
                sub = cache[i - 10] * ((seq_t)741);
            }
            seq_t val = term - sub;
            if (i <= 10) {
                val += coeffA[(int)i];
            }
            cache[i] = val;
        }
        cache_len = need;
    }
    return cache[(size_t)n];
}
