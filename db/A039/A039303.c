/*
 * A039303  Number of distinct quadratic residues mod 6^n.
 * https://oeis.org/A039303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50*a(n-2) - 553*a(n-4) + 1800*a(n-6) - 1296*a(n-8) for n
 *            >= 9. Initial values: a(0)=1, a(1)=4, a(2)=8, a(3)=33, a(4)=124,
 *            a(5)=644, a(6)=3288, a(7)=18883, a(8)=108284
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* Length of cache array. */
    static idx_t cached_max = 0;    /* Highest index already computed. */
    
    /* Initial terms for n = 0..8. */
    static const seq_t init_vals[9] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)8,
        (seq_t)33,
        (seq_t)124,
        (seq_t)644,
        (seq_t)3288,
        (seq_t)18883,
        (seq_t)108284
    };
    
    /* Allocate cache if not yet done, and copy initial values. */
    if (cache_len == 0)
    {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;   /* Should not happen for valid builds. */
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
        cached_max = 8;
    }
    
    /* If requested index already cached, return it. */
    if (n < cache_len)
        return cache[n];
    
    /* Expand cache to accommodate up to n. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;  /* Allocation failed: return 0 as fallback. */
    cache = tmp;
    cache_len = new_len;
    
    /* Compute missing values from cached_max+1 up to n. */
    for (idx_t idx = cached_max + 1; idx <= n; ++idx)
    {
        seq_t v1 = mul_small(cache[idx - 2], 50);
        seq_t v2 = mul_small(cache[idx - 4], 553);
        seq_t v3 = mul_small(cache[idx - 6], 1800);
        seq_t v4 = mul_small(cache[idx - 8], 1296);
        seq_t value = v1;
        value = value - v2;
        value = value + v3;
        value = value - v4;
        cache[idx] = value;
    }
    cached_max = n;
    return cache[n];
}
