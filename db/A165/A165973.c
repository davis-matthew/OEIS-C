/*
 * A165973  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165973: a(n) = 24·(a(n-1)+⋯+a(n-9)) − 300·a(n-10) + c(n)
   where c(0)=1, c(1)=⋯=c(9)=2, c(10)=1, c(n)=0 for n>10.
   First ten values:
   a(0)=1, a(1)=26, a(2)=650, a(3)=16250, a(4)=406250,
   a(5)=10156250, a(6)=253906250, a(7)=6347656250,
   a(8)=158691406250, a(9)=3967285156250. */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t COEF[11] = {
        (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)1
    };

    if (cache_size <= n) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 9; ++j)
                if (i >= j) sum += cache[i - j];
            seq_t term10 = (i >= 10) ? mul_small(cache[i - 10], 300U) : (seq_t)0;
            seq_t val    = mul_small(sum, 24U) - term10;
            if (i <= 10) val += COEF[i];
            cache[i] = val;
        }
    }
    return cache[n];
}
