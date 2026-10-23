/*
 * A041473  Denominators of continued fraction convergents to sqrt(252).
 * https://oeis.org/A041473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 254*a(n-4) - a(n-8) for n > 7 Initial values: a(0)=1,
 *            a(1)=1, a(2)=7, a(3)=8, a(4)=247, a(5)=255, a(6)=1777, a(7)=2032
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    /* initialise cache and first eight terms on first call */
    if (!cache) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 7;
        cache[3] = 8;
        cache[4] = 247;
        cache[5] = 255;
        cache[6] = 1777;
        cache[7] = 2032;
        max_computed = 7;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* return cached value if already computed */
    if (n <= max_computed)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 254 * cache[i - 4] - cache[i - 8];
    }
    max_computed = n;
    return cache[n];
}
