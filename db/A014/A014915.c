/*
 * A014915  a(1)=1, a(n) = n*3^(n-1) + a(n-1).
 * https://oeis.org/A014915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*3 + 3*3^2 + … + n*3^(n-1) Recurrence: a(1)=1,
 *            a(n)=a(n-1)+n*3^(n-1) (n≥2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   ensure_capacity, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_small.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Handle n=0 gracefully – the sequence is defined from n=1. */
    if (n == 0) return (seq_t)0;

    /* Static cache to allow random access order. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* largest n cached (≥1) */
    static idx_t cache_cap = 0;      /* capacity of cache array */
    static seq_t last_pow3 = 0;      /* 3^(cache_len-1) */

    /* Allocate and initialise cache on first call. */
    if (cache == NULL) {
        ensure_capacity(2, &cache_cap, (void **)&cache, sizeof(seq_t));
        if (cache_cap == 0) {   /* safety check */
            cache_cap = 2;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        }
        cache[1] = (seq_t)1;        /* a(1) = 1 */
        cache_len = 1;
        last_pow3 = (seq_t)1;       /* 3^(1-1) = 1 */
    }

    /* Return cached value if already computed. */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Expand cache to accommodate up to n. */
    ensure_capacity(n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
    if (cache_cap < n + 1) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* Compute missing terms up to n. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Update last_pow3 to 3^(k-1). */
        last_pow3 = mul_small(last_pow3, 3);

        /* term = k * 3^(k-1).  k fits in unsigned int for the intended limits. */
        seq_t term = mul_small(last_pow3, (unsigned int)k);

        /* a(k) = a(k-1) + term. */
        cache[k] = cache[k - 1] + term;
    }

    cache_len = n;
    return cache[n];
}
