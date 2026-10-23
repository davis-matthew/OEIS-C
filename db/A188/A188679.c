/*
 * A188679  Partial sums of binomial(3n,n)^2.
 * https://oeis.org/A188679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} binomial(3k,k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {           /* allocation failure – compute on the fly */
            cache = NULL;
            cache_cap = 0;
            seq_t sum = 0;
            for (idx_t k = 0; k <= n; ++k) {
                seq_t b = binomial(3 * k, k);
                sum += b * b;
            }
            return sum;
        }
        cache = tmp;
        for (idx_t i = cache_cap; i <= n; ++i) {
            seq_t b = binomial(3 * i, i);
            seq_t term = b * b;
            if (i == 0)
                cache[0] = term;
            else
                cache[i] = cache[i - 1] + term;
        }
        cache_cap = n + 1;
    }
    return cache[n];
}
