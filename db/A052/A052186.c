/*
 * A052186  Number of permutations of [n] with no strong fixed points.
 * https://oeis.org/A052186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0)=1, a(1)=0, for n>1 a(n) = (n-2)*a(n-1) + Σ_{k=0}^{n-1}
 *            a(k)*a(n-1-k) + Σ_{k=0}^{n-2} a(k)*a(n-2-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)0;
            else {
                seq_t term = (seq_t)(i - 2) * cache[i - 1];
                seq_t sum1 = 0;
                seq_t sum2 = 0;
                for (idx_t k = 0; k <= i - 1; ++k)
                    sum1 += cache[k] * cache[i - 1 - k];
                for (idx_t k = 0; k <= i - 2; ++k)
                    sum2 += cache[k] * cache[i - 2 - k];
                cache[i] = term + sum1 + sum2;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
