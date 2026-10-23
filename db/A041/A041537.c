/*
 * A041537  Denominators of continued fraction convergents to sqrt(285).
 * https://oeis.org/A041537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4862*a(n-6) - a(n-12) for n>11 Initial values: a(0)=1,
 *            a(1)=1, a(2)=8, a(3)=17, a(4)=127, a(5)=144, a(6)=4735,
 *            a(7)=4879, a(8)=38888, a(9)=82655, a(10)=617473, a(11)=700128
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initial 12 terms of the sequence */
    static const seq_t init[12] = {
        1ULL, 1ULL, 8ULL, 17ULL, 127ULL, 144ULL,
        4735ULL, 4879ULL, 38888ULL, 82655ULL,
        617473ULL, 700128ULL
    };

    /* allocate cache if first call */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i) {
            cache[i] = init[i];
        }
    }

    /* grow cache to at least n+1 */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed, fallback to return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                cache[i] = (seq_t)4862 * cache[i - 6] - cache[i - 12];
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
