/*
 * A202833  Expansion of e.g.f.: exp(9*x/(1-x)) / sqrt(1-x^2).
 * https://oeis.org/A202833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+8)*a(n-1) + (n-1)*(n+8)*a(n-2) - (n-1)*(n-2)^2*a(n-3),
 *            initial values: a(0)=1, a(1)=9, a(2)=100. This recurrence is
 *            derived from a(n) = ( Σ_{k=0..⌊n/2⌋} 3^(n-2k)/2^k *
 *            n!/((n-2k)!k!) )^2. The recurrence allows an O(n) implementation
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   mul_seq_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* uintmax_t, etc. */
#include "../utils/mul_seq_fast.h"

seq_t a(idx_t n)
{
    /* static cache: values of a(0)..a(last_computed) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of stored entries */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Allocate initial cache if needed */
    if (cache == NULL)
    {
        cache_cap = (n > 30 ? n * 2 : 32);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    else if ((idx_t)(n + 1) > cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((idx_t)(n + 1) > new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 9;
        else if (i == 2)
            cache[2] = 100;
        else
        {
            seq_t t1 = mul_seq_fast((seq_t)(i + 8), cache[i - 1]);

            seq_t coeff2 = mul_seq_fast((seq_t)(i - 1), (seq_t)(i + 8));
            seq_t t2 = mul_seq_fast(coeff2, cache[i - 2]);

            seq_t coeff3_part = mul_seq_fast((seq_t)(i - 2), (seq_t)(i - 2));
            seq_t coeff3 = mul_seq_fast((seq_t)(i - 1), coeff3_part);
            seq_t t3 = mul_seq_fast(coeff3, cache[i - 3]);

            cache[i] = t1 + t2 - t3;   /* recurrence */
        }
    }
    cache_len = n + 1;
    return cache[n];
}
