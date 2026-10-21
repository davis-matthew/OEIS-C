/*
 * A064651  a(n) = ceiling(a(n-1)/2) + a(n-2) with a(0)=0 and a(1)=1.
 * https://oeis.org/A064651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(a(n-1)/2) + a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 89, u64 n <= 179,
 *            u128 n <= 358, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow cache if needed and compute any new terms */
    if ((idx_t)(n + 1) > cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fall back to no caching */
            cache = NULL;
            cache_size = 0;
        } else {
            cache = tmp;
            cache_size = new_size;
            /* Initialise new elements (default 0 for integers, empty for big integers) */
            for (idx_t i = old_size; i < new_size; ++i) {
                if (i == 0) {
                    cache[0] = (seq_t)0;
                } else if (i == 1) {
                    cache[1] = (seq_t)1;
                } else {
                    seq_t t = cache[i - 1] + (seq_t)1;
                    t = t / (seq_t)2;          /* ceil division by 2 */
                    cache[i] = t + cache[i - 2];
                }
            }
        }
    }

    return cache[n];
}
