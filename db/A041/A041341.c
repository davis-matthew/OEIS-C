/*
 * A041341  Denominators of continued fraction convergents to sqrt(184).
 * https://oeis.org/A041341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 48670*a(n-12) - a(n-24) Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=7, a(4)=16, a(5)=23, a(6)=62, a(7)=85, a(8)=232,
 *            a(9)=781, a(10)=1013, a(11)=1794, a(12)=47657, a(13)=49451,
 *            a(14)=97108, a(15)=340775, a(16)=778658, a(17)=1119433,
 *            a(18)=3017524, a(19)=4136957, a(20)=11291438, a(21)=3801...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial 24 terms */
    static const seq_t init[24] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)16, (seq_t)23,
        (seq_t)62, (seq_t)85, (seq_t)232, (seq_t)781, (seq_t)1013,
        (seq_t)1794, (seq_t)47657, (seq_t)49451, (seq_t)97108,
        (seq_t)340775, (seq_t)778658, (seq_t)1119433, (seq_t)3017524,
        (seq_t)4136957, (seq_t)11291438, (seq_t)38011271,
        (seq_t)49302709, (seq_t)87313980
    };
    const idx_t init_len = 24;

    /* cache for indices >= init_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached terms (>= init_len) */

    if (n < init_len)
        return init[(idx_t)n];

    /* n >= init_len */
    idx_t needed = n - init_len + 1;        /* number of terms to cache up to n */

    if (cache_len < needed) {
        /* enlarge cache */
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        /* compute new terms */
        for (idx_t k = cache_len; k < needed; ++k) {
            idx_t i = init_len + k;  /* actual index in the sequence */

            seq_t term12;
            if (i - 12 < init_len)
                term12 = init[(idx_t)(i - 12)];
            else
                term12 = cache[(idx_t)(i - 12 - init_len)];

            seq_t term24;
            if (i - 24 < init_len)
                term24 = init[(idx_t)(i - 24)];
            else
                term24 = cache[(idx_t)(i - 24 - init_len)];

            cache[k] = (seq_t)48670 * term12 - term24;
        }
        cache_len = needed;
    }

    return cache[n - init_len];
}
