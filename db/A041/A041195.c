/*
 * A041195  Denominators of continued fraction convergents to sqrt(108).
 * https://oeis.org/A041195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2702*a(n-8) - a(n-16) (n >= 16; a(0..15) given)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = 0;

    /* initialise cache with first 16 terms */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) exit(1);
        static const seq_t init[16] = {
            1, 2, 3, 5, 23, 28, 51, 130,
            2651, 5432, 8083, 13515, 62143, 75658, 137801, 351260
        };
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init[i];
        last = 15;
    }

    if (n < 16)
        return cache[(size_t)n];

    size_t need = (size_t)n + 1;
    if (need > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = need;
    }

    for (size_t i = last + 1; i <= (size_t)n; ++i)
        cache[i] = 2702 * cache[i - 8] - cache[i - 16];

    last = (size_t)n;
    return cache[(size_t)n];
}
