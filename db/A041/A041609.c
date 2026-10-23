/*
 * A041609  Denominators of continued fraction convergents to sqrt(322).
 * https://oeis.org/A041609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 646*a(n-4) - a(n-8) (n > 7) Initial values: a(0)=1,
 *            a(1)=1, a(2)=17, a(3)=18, a(4)=629, a(5)=647, a(6)=10981,
 *            a(7)=11628
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system (unsigned integer types) */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms currently stored (next free index) */

    /* Grow the cache if we need a value beyond what we have */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;                 /* need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                               /* allocation failure -> fatal */
        cache = tmp;

        /* Initial eight terms */
        static const seq_t base[8] = {
            (seq_t)1, (seq_t)1, (seq_t)17, (seq_t)18,
            (seq_t)629, (seq_t)647, (seq_t)10981, (seq_t)11628
        };

        /* Compute any new terms up to n */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < 8) {
                cache[i] = base[i];
            } else {
                /* recurrence: a(i) = 646*a(i-4) - a(i-8) */
                cache[i] = cache[i-4] * (seq_t)646 - cache[i-8];
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
