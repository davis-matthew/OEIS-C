/*
 * A042467  Denominators of continued fraction convergents to sqrt(761).
 * https://oeis.org/A042467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1600*a(n-7) + a(n-14) for n >= 14; Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=12, a(5)=17, a(6)=29,
 *            a(7)=1583, a(8)=1612, a(9)=3195, a(10)=8002, a(11)=19199,
 *            a(12)=27201, a(13)=46400
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init[14] = {
        1, 1, 2, 5, 12, 17, 29,
        1583, 1612, 3195, 8002, 19199, 27201, 46400
    };

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)cache_size * sizeof(seq_t));
        /* The build system guarantees realloc succeeds; no error handling needed. */

        for (idx_t i = old_size; i < cache_size; ++i) {
            if (i < 14) {
                cache[i] = init[i];
            } else {
                cache[i] = cache[i-7] * (seq_t)1600 + cache[i-14];
            }
        }
    }
    return cache[n];
}
