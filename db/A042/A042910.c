/*
 * A042910  Numerators of continued fraction convergents to sqrt(987).
 * https://oeis.org/A042910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 754*a(n-4) - a(n-8) Initial values: a(0)=31, a(1)=63,
 *            a(2)=157, a(3)=377, a(4)=23531, a(5)=47439, a(6)=118409,
 *            a(7)=284257 Computation is memoised; any call order is supported
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* pointer to cached values   */
    static size_t cache_sz = 0;     /* number of cached elements */

    /* Expand cache enough to include n */
    if ((size_t)n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp;

        if (cache == NULL)
            tmp = (seq_t *)malloc(new_sz * sizeof(*cache));
        else
            tmp = (seq_t *)realloc(cache, new_sz * sizeof(*cache));

        if (!tmp)                /* allocation failed */
            return (seq_t)0;     /* caller will treat as 0 */
        cache = tmp;

        for (size_t i = cache_sz; i < new_sz; ++i) {
            if (i == 0)          cache[i] = 31;
            else if (i == 1)     cache[i] = 63;
            else if (i == 2)     cache[i] = 157;
            else if (i == 3)     cache[i] = 377;
            else if (i == 4)     cache[i] = 23531;
            else if (i == 5)     cache[i] = 47439;
            else if (i == 6)     cache[i] = 118409;
            else if (i == 7)     cache[i] = 284257;
            else
                cache[i] = (seq_t)754 * cache[i-4] - cache[i-8];
        }
        cache_sz = new_sz;
    }

    return cache[(size_t)n];
}
