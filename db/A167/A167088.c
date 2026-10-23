/*
 * A167088  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1. For 1 ≤ n ≤ 12: a(n) = 33*∑_{k=1}^n a(n-k) + 2. For n =
 *            13: a(13) = 33*∑_{k=1}^{12} a(13-k) – 561*a(0) + 1. For n ≥ 14:
 *            a(n) = 33*∑_{k=1}^{12} a(n-k) – 561*a(n-13)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    size_t N = (size_t)n;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    if (N >= cache_len) {
        if (N + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < N + 1) new_cap = N + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = cache_len; i <= N; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i <= 12) {
                seq_t sum = 0;
                for (size_t k = 1; k <= i; ++k) sum += cache[i - k];
                val = (seq_t)33 * sum + (seq_t)2;
            } else if (i == 13) {
                seq_t sum = 0;
                for (size_t k = 1; k <= 12; ++k) sum += cache[i - k];
                val = (seq_t)33 * sum - (seq_t)561 * cache[0] + (seq_t)1;
            } else { /* i >= 14 */
                seq_t sum = 0;
                for (size_t k = 1; k <= 12; ++k) sum += cache[i - k];
                val = (seq_t)33 * sum - (seq_t)561 * cache[i - 13];
            }
            cache[i] = val;
        }
        cache_len = N + 1;
    }
    return cache[N];
}
