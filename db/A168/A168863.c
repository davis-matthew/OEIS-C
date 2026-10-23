/*
 * A168863  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n<20: a(n)=2+40*∑_{i=0}^{n-1} a(i); for n≥20:
 *            a(n)=40*∑_{i=n-19}^{n-1} a(i)-820*a(n-20)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached elements */

    if (n >= cache_len) {        /* need to extend the cache */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed, return zero */
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
                continue;
            }
            if (i < 20) {                      /* 1 ≤ i < 20 */
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j)
                    sum += cache[j];
                cache[i] = mul_small(sum, 40);
                cache[i] += 2;
            } else {                           /* i ≥ 20 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 19; ++j)
                    sum += cache[i - j];
                seq_t temp = mul_small(sum, 40);
                seq_t sub  = mul_small(cache[i - 20], 820);
                cache[i] = temp - sub;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
