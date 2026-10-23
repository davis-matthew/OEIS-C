/*
 * A041295  Denominators of continued fraction convergents to sqrt(160).
 * https://oeis.org/A041295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1442*a(n-8) - a(n-16), with initial values a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=17, a(5)=20, a(6)=37, a(7)=57,
 *            a(8)=1405, a(9)=1462, a(10)=2867, a(11)=4329, a(12)=24512,
 *            a(13)=28841, a(14)=53353, a(15)=82194
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initial 16 terms, cast to seq_t to support all sizes */
    static const seq_t init[16] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)17, (seq_t)20, (seq_t)37, (seq_t)57,
        (seq_t)1405, (seq_t)1462, (seq_t)2867, (seq_t)4329,
        (seq_t)24512, (seq_t)28841, (seq_t)53353, (seq_t)82194
    };

    /* if already computed, return */
    if (n < cache_size)
        return cache[(size_t)n];

    /* extend cache up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – crash is acceptable in this context */
        abort();
    }
    cache = tmp;
    /* compute new entries */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i < 16) {
            cache[i] = init[i];
        } else {
            cache[i] = (cache[i-8] * (seq_t)1442) - cache[i-16];
        }
    }
    cache_size = new_size;
    return cache[n];
}
