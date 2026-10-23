/*
 * A170432  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Denominator D(t) = 1 – 37t – 37t^2 – … – 37t^43 + 703t^44
 *          Numerator N(t)   = 1 + 2t + 2t^2 + … + 2t^43 + t^44
 * The sequence satisfies:
 *   a(n) – 37·∑_{k=1}^{43} a(n−k) + 703·a(n−44) = coeff_N(n)
 *   coeff_N(0)=1, coeff_N(n)=2 for 1≤n≤43, coeff_N(44)=1, otherwise 0.
 * All a(m) with m<0 are taken as 0.
 * This implementation memoizes computed values up to the requested index.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static size_t cache_size = 0;   /* number of entries currently cached (>=1) */
    static seq_t *cache = NULL;     /* array of cached values 0..cache_size-1 */

    if (n < cache_size)            /* already computed */
        return cache[(size_t)n];

    /* extend cache to hold indices up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;            /* memory allocation failed – return 0 as safe default */
    cache = tmp;

    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        /* coefficient of t^i in N(t) */
        seq_t coeff;
        if (i == 0 || i == 44)
            coeff = 1;
        else if (i >= 1 && i <= 43)
            coeff = 2;
        else
            coeff = 0;

        /* compute 37 * sum_{k=1}^{43} a(i−k) */
        seq_t sum = 0;
        for (unsigned k = 1; k <= 43; ++k) {
            if (i >= (idx_t)k)
                sum += cache[(size_t)(i - k)];
        }
        seq_t sum37 = mul_small(sum, 37);

        /* assemble a(i) */
        seq_t value = coeff + sum37;
        if (i >= 44)
            value -= mul_small(cache[(size_t)(i - 44)], 703);

        cache[(size_t)i] = value;
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
