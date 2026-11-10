/*
 * A201684  a(n) = 2*A052186(n) - n!.
 * https://oeis.org/A201684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A052186(n) - n!, with A052186(0)=1, A052186(1)=0,
 *            A052186(n) = (n-2)*A052186(n-1) + Sum_{k=0..n-1}
 *            A052186(k)*A052186(n-1-k) + Sum_{k=0..n-2}
 *            A052186(k)*A052186(n-2-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 436 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A052186_a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = mem_resize(seq_t, cache, new_size);
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

seq_t a(idx_t n)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= n; ++i)
        f *= (seq_t)i;
    return (seq_t)2 * A052186_a(n) - f;
}
