/*
 * A099444  A Chebyshev transform of Fib(2n+2).
 * https://oeis.org/A099444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - a(n-4), a(0) = 1, a(1) =
 *            3, a(2) = 7, a(3) = 15
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   ensure_cache_capacity, mul3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the 4‑term linear recurrence derived from the generating function
   (1+x²)/(1-3x+3x²-3x³+x⁴).  The function memoises already computed values
   so it works correctly regardless of the order of calls. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_cache_capacity.h"
#include "../utils/mul3.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* last valid index in cache */
    static idx_t cache_cap  = 0;  /* allocated size of cache */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        ensure_cache_capacity(&cache_cap, 4, &cache);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)15;
        cache_size = 3;
    }

    if (n <= cache_size)
        return cache[n];

    /* Extend cache up to index n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        ensure_cache_capacity(&cache_cap, i + 1, &cache);
        seq_t term = mul3(cache[i-1]) - mul3(cache[i-2]) + mul3(cache[i-3]) - cache[i-4];
        cache[i] = term;
    }
    cache_size = n;
    return cache[n];
}
