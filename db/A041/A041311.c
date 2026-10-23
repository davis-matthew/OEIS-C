/*
 * A041311  Denominators of continued fraction convergents to sqrt(168).
 * https://oeis.org/A041311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 26*a(n-2) - a(n-4) (n >= 4) a(0) = 1, a(1) = 1, a(2) =
 *            25, a(3) = 26
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t capacity = 0;  /* allocated size of cache */
    static size_t computed = 0;  /* number of terms currently stored (next free index) */

    /* Ensure the cache can hold at least n+1 terms */
    if (n + 1 > capacity) {
        size_t newcap = capacity ? capacity * 2 : 1;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* Compute terms up to n */
    while (computed <= n) {
        switch (computed) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)1; break;
            case 2: cache[2] = (seq_t)25; break;
            case 3: cache[3] = (seq_t)26; break;
            default:
                /* a(k) = 26*a(k-2) - a(k-4)  for k >= 4 */
                cache[computed] =
                    (seq_t)( (seq_t)26 * cache[computed-2] - cache[computed-4] );
                break;
        }
        ++computed;
    }

    return cache[n];
}
