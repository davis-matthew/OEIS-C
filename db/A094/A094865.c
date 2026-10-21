/*
 * A094865  Expansion of x^3/((1-3*x+x^2)*(1-5*x+5*x^2)).
 * https://oeis.org/A094865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) with initial
 *            values a(0)=0, a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation cache */
    static idx_t cache_len = 0;   /* number of elements stored */

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Ensure space for all values up to n */
    size_t needed = (size_t)(n + 1);

    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
    } else if (needed > cache_len) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    }

    /* Compute new terms */
    for (idx_t i = cache_len; i < (idx_t)needed; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            cache[i] = (seq_t)0;
        } else if (i == 3) {
            cache[i] = (seq_t)1;
        } else {
            cache[i] = 8 * cache[i - 1]
                     - 21 * cache[i - 2]
                     + 20 * cache[i - 3]
                     - 5 * cache[i - 4];
        }
    }

    cache_len = (idx_t)needed;
    return cache[n];
}
