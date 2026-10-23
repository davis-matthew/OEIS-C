/*
 * A041258  Numerators of continued fraction convergents to sqrt(141).
 * https://oeis.org/A041258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 190*a(n-4) - a(n-8) with initial values a(0)=11, a(1)=12,
 *            a(2)=83, a(3)=95, a(4)=2173, a(5)=2268, a(6)=15781, a(7)=18049
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;

    /* handle the first 8 terms directly */
    if (n < 8) {
        switch (n) {
            case 0: return (seq_t)11;
            case 1: return (seq_t)12;
            case 2: return (seq_t)83;
            case 3: return (seq_t)95;
            case 4: return (seq_t)2173;
            case 5: return (seq_t)2268;
            case 6: return (seq_t)15781;
            case 7: return (seq_t)18049;
        }
    }

    /* initialise cache on first use */
    if (!cache) {
        cache_size = 8;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        cache[0] = (seq_t)11;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)83;
        cache[3] = (seq_t)95;
        cache[4] = (seq_t)2173;
        cache[5] = (seq_t)2268;
        cache[6] = (seq_t)15781;
        cache[7] = (seq_t)18049;
    }

    /* extend cache up to the requested n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed – fall back to dynamic calc */
            /* compute the missing terms without cache */
            seq_t result = cache[7];
            for (idx_t k = cache_size; k <= n; ++k) {
                result = (seq_t)190 * cache[k - 4] - cache[k - 8];
                cache = (seq_t *)realloc(cache, (size_t)(k + 1) * sizeof(seq_t));
                cache[k] = result;
                cache_size = k + 1;
            }
            return result;
        }
        cache = tmp;
        for (idx_t k = cache_size; k <= n; ++k) {
            cache[k] = (seq_t)190 * cache[k - 4] - cache[k - 8];
        }
        cache_size = new_size;
    }

    return cache[n];
}
