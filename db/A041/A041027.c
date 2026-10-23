/*
 * A041027  Denominators of continued fraction convergents to sqrt(18).
 * https://oeis.org/A041027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=1, a(1)=4; for n>=2, a(n)=((n%2==0)?8:4)*a(n-1)+a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently stored */

    size_t new_size = (size_t)(n + 1);

    /* Ensure the cache is large enough for the requested index */
    if (cache_size <= n) {
        /* Allocate or grow the cache */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache)            /* allocation failure; return zero as a safe fallback */
            return (seq_t)0;

        /* Initialise the first one or two terms if they have not been set yet */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            if (n >= 1) {
                cache[1] = (seq_t)4;
                cache_size = 2;
            } else {
                cache_size = 1;
            }
        } else if (cache_size == 1 && n >= 1) {
            cache[1] = (seq_t)4;
            cache_size = 2;
        }

        /* Compute any missing terms up to n */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t coeff = ((i & 1u) == 0) ? (seq_t)8 : (seq_t)4;
            cache[i] = coeff * cache[i - 1] + cache[i - 2];
        }

        cache_size = new_size;
    }

    return cache[n];
}
