/*
 * A068639  a(0) = 0, a(n) = a(n-1) + (-1)^p(n) for n >= 1, where p(n) =
 *          highest power of 2 dividing n.
 * https://oeis.org/A068639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (-1)^v2(n) with a(0) = 0 (v2 = exponent of 2 in
 *            n)
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline int v2(idx_t n)
{
    int e = 0;
    while ((n & 1) == 0 && n != 0) {
        ++e;
        n >>= 1;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* lazy memoization cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size <= n) {
        idx_t old_size = cache_size;

        if (cache == NULL) {
            /* first allocation up to n */
            cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
            cache[0] = (seq_t)0;
            cache_size = 1;
            old_size   = 1;
        } else {
            /* extend existing cache */
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
            cache_size = n + 1;
        }

        for (idx_t i = old_size; i <= n; ++i) {
            int e = v2(i);
            if ((e & 1) == 0)          /* even exponent → add 1 */
                cache[i] = cache[i - 1] + (seq_t)1;
            else                       /* odd exponent  → subtract 1 */
                cache[i] = cache[i - 1] - (seq_t)1;
        }
    }

    return cache[n];
}
