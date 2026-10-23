/*
 * A184689  1/3 the number of n X 4 0..2 arrays with no element equal both to
 *          the element above and to the element to its left.
 * https://oeis.org/A184689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 61*a(n-1) - 266*a(n-2) + 416*a(n-3) - 256*a(n-4) (n≥5)
 *            with a(1)=27, a(2)=1536, a(3)=86688, a(4)=4890528
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding the computed values a(1)…a(k) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)
        return 0;                     /* offset starts at 1 */

    if (cache == NULL) {
        /* initialise for indices 0..4 (index 0 unused) */
        cache_size = 4;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 27;
        cache[2] = 1536;
        cache[3] = 86688;
        cache[4] = 4890528;
    }

    if (n <= cache_size)
        return cache[n];

    /* extend the cache up to n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] = (seq_t)61 * cache[i - 1]
                 - (seq_t)266 * cache[i - 2]
                 + (seq_t)416 * cache[i - 3]
                 - (seq_t)256 * cache[i - 4];
    }
    cache_size = n;
    return cache[n];
}
