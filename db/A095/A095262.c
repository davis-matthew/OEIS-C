/*
 * A095262  A sequence derived from a truncated Pascal's Triangle matrix.
 * https://oeis.org/A095262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 9*a(n+2) – 26*a(n+1) + 24*a(n) with a(1)=2, a(2)=21,
 *            a(3)=137
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                 /* sequence defined for n ≥ 1 */
        return (seq_t)0;

    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0;   /* total allocated slots (0..capacity-1) */
    static idx_t   cache_max = 0;   /* largest n currently stored */

    /* expand cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;           /* slots 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                                       /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)2;
        else if (i == 2)
            cache[2] = (seq_t)21;
        else if (i == 3)
            cache[3] = (seq_t)137;
        else
            cache[i] = 9 * cache[i-1] - 26 * cache[i-2] + 24 * cache[i-3];
    }
    cache_max = n;
    return cache[n];
}
