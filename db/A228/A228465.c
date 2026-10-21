/*
 * A228465  Recurrence a(n) = a(n-1) + 2^n*a(n-2) with a(0)=0, a(1)=1.
 * https://oeis.org/A228465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2^n * a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   pow_uint, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of computed terms   */
    static size_t cache_len  = 0;      /* number of terms stored    */
    static size_t cache_cap  = 0;      /* capacity of the array     */
    static seq_t pow2_current = 0;     /* holds 2^i for current i   */

    /* initialise on first call */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = seq_alloc(cache_cap);
        if (!cache) return (seq_t)0;  /* allocation failed            */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
        pow2_current = pow_uint(2, 1);   /* 2^1  – will be doubled to 2^2 on first iteration */
    }

    /* if requested value already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* ensure capacity for n+1 elements */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
        if (!new_cache) return (seq_t)0;  /* allocation failed            */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* update 2^i */
        pow2_current = seq_mul(pow2_current, (seq_t)2);

        /* a(i) = a(i-1) + 2^i * a(i-2) */
        seq_t prod = seq_mul(pow2_current, cache[i - 2]);   /* 2^i * a(i-2) */
        cache[i]   = cache[i - 1] + prod;
    }
    cache_len = n + 1;
    return cache[n];
}
