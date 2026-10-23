/*
 * A042448  Numerators of continued fraction convergents to sqrt(752).
 * https://oeis.org/A042448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 9214*a(n-8) - a(n-16) Initial values: a(0) = 27, a(1) =
 *            55, a(2) = 137, a(3) = 192, a(4) = 521, a(5) = 713, a(6) = 1947,
 *            a(7) = 4607, a(8) = 250725, a(9) = 506057, a(10) = 1262839,
 *            a(11) = 1768896, a(12) = 4800631, a(13) = 6569527, a(14) =
 *            17939685, a(15) = 42448897
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for all computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cached entries (max index +1) */

    /* Grow the cache to contain the requested term */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (cache == NULL)
                        ? (seq_t *)malloc(new_size * sizeof(seq_t))
                        : (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;

        /* Initial 16 terms of the sequence */
        static const seq_t init[16] = {
            27, 55, 137, 192, 521, 713,
            1947, 4607, 250725, 506057,
            1262839, 1768896, 4800631, 6569527,
            17939685, 42448897
        };

        /* Compute newly needed terms */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 16) {
                cache[i] = init[(size_t)i];
            } else {
                seq_t prod = ((seq_t)9214 * cache[i - 8]) - cache[i - 16];
                cache[i] = prod;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
