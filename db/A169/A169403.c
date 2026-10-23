/*
 * A169403  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) - 4*a(n‑1) - … - 4*a(n‑31) + 10*a(n‑32) =
 *            b(n) where b(0)=1, b(1…31)=2, b(32)=1, and b(n)=0 for n>32. This
 *            recurrence follows from the generating function
 *            (1+2*∑_{k=1}^{31}x^k+x^32)/(1‑4*∑_{k=1}^{31}x^k+10*x^32)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    size_t need = (size_t)n + 1;

    if (cache == NULL) {
        cache_cap = need;
        cache     = seq_alloc_array(cache_cap);
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
    } else if (need > cache_cap) {
        /* Allocate a larger array, copy old values, and zero the new slots. */
        seq_t *new_cache = seq_alloc_array(need);
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        for (size_t i = cache_len; i < need; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = need;
    }

    /* Compute any values that are not yet present. */
    for (size_t i = cache_len; i < need; ++i) {
        seq_t sum31 = 0;
        size_t kmax = (i < 31) ? i : 31;
        for (size_t k = 1; k <= kmax; ++k)
            sum31 += cache[i - k];

        seq_t rhs;
        if (i == 0)
            rhs = 1;
        else if (i <= 31)
            rhs = 2;
        else if (i == 32)
            rhs = 1;
        else
            rhs = 0;

        seq_t val = rhs + sum31 * 4;
        if (i >= 32)
            val -= cache[i - 32] * 10;

        cache[i] = val;
    }

    cache_len = need;
    return cache[(size_t)n];
}
