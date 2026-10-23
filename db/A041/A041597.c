/*
 * A041597  Denominators of continued fraction convergents to sqrt(316).
 * https://oeis.org/A041597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 25598*a(n-8) - a(n-16) for n>15
   Initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=9, a(4)=76, a(5)=161,
   a(6)=559, a(7)=720, a(8)=25039, a(9)=25759, a(10)=102316,
   a(11)=230391, a(12)=1945444, a(13)=4121279, a(14)=14309281,
   a(15)=18430560 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* base terms */
    static const seq_t init[16] = {
        1ULL, 1ULL, 4ULL, 9ULL, 76ULL, 161ULL, 559ULL, 720ULL,
        25039ULL, 25759ULL, 102316ULL, 230391ULL, 1945444ULL,
        4121279ULL, 14309281ULL, 18430560ULL
    };

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        for (idx_t i = 0; i < 16; ++i) {
            cache[i] = init[i];
        }
        cache_len = 16;
    }

    /* expand cache capacity if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 16) continue;                 /* already initialised */

        /* a(i) = 25598 * a(i-8) - a(i-16) */
        seq_t prod = mul_small(cache[i - 8], 25598U);
        prod -= cache[i - 16];
        cache[i] = prod;
    }

    if (n + 1 > cache_len) cache_len = n + 1;

    return cache[n];
}
