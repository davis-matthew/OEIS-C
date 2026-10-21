/*
 * A099427  a(1) = 1; for n > 1, a(n) = 1 + greatest common divisor of n and
 *          a(n-1).
 * https://oeis.org/A099427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(1) = 1; for n > 1, a(n) = 1 + gcd(n, a(n-1))
 * Range:     u8 n <= 941, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms; index 0 is unused */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* highest index already stored */

    /* Initialise cache with a(1) if necessary */
    if (!cache) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));   /* indices 0 and 1 */
        if (!cache) return (seq_t)0;                 /* allocation failure */
        cache[1] = (seq_t)1;
        cached_len = 1;
    }

    /* Return cached value if already present */
    if (n <= cached_len)
        return cache[n];

    /* Expand the cache to include the requested index */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                       /* allocation failure */
    cache = tmp;

    /* Compute terms from the first missing index up to n */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        seq_t g = gcd((seq_t)i, cache[i - 1]);
        cache[i] = g + (seq_t)1;
    }

    cached_len = n;
    return cache[n];
}
