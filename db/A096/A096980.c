/*
 * A096980  Expansion of (1+3*x)/(1-2*x-7*x^2).
 * https://oeis.org/A096980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size */
    static idx_t last = (idx_t)-1;  /* highest index computed */

    /* first call: allocate for at least two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure => 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        last = 1;
    }

    /* extend cache if needed */
    if (n > last) {
        /* ensure capacity up to n+1 elements */
        size_t needed = (size_t)n + 1;
        if (needed > cache_size) {
            size_t new_size = cache_size;
            while (new_size < needed) new_size <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure => 0 */
            cache = tmp;
            cache_size = new_size;
        }

        /* compute missing terms */
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)2 + cache[i - 2] * (seq_t)7;
        }
        last = n;
    }

    return cache[(size_t)n];
}
