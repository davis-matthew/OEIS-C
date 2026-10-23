/*
 * A016318  Expansion of 1/((1-2x)(1-8x)(1-11x)).
 * https://oeis.org/A016318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^(n+2) + 2^(n+1) - 96*8^n)/27 Recurrence: a(0)=1,
 *            a(1)=21 a(n)=19*a(n-1) - 88*a(n-2) + 2^n (n>=2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/ensure_cache.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last_filled = 1;  /* last index fully computed */

    /* Handle the first term manually */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialize the first two entries if this is the first use */
    if (last_filled == 1 && cap >= 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
    }

    /* If the requested term is already computed, return it */
    if (n <= last_filled) {
        return cache[n];
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        seq_t pow2i   = pow2(i);                /* 2^i */
        seq_t prev1   = cache[i - 1];           /* a(i-1) */
        seq_t prev2   = cache[i - 2];           /* a(i-2) */
        cache[i] = prev1 * (seq_t)19 - prev2 * (seq_t)88 + pow2i;
    }

    last_filled = n;
    return cache[n];
}
