/*
 * A207077  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x+497)^2 = y^2.
 * https://oeis.org/A207077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-1) + 6*a(n-9) -
 *            6*a(n-10) - a(n-18) + a(n-19)
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 84, u64 n <= 198, u128 n <= 424,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Derived from the rational generating function of A207077) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1)..a(19)  (offset 1) */
    static const seq_t init[20] = {
        0, /* dummy for 0-index */
        0,   /* a(1) */
        91,  /* a(2) */
        216, /* a(3) */
        355, /* a(4) */
        387, /* a(5) */
        528, /* a(6) */
        568, /* a(7) */
        795, /* a(8) */
        1120,/* a(9) */
        1491,/* a(10) */
        1960,/* a(11) */
        2635,/* a(12) */
        3408,/* a(13) */
        3588,/* a(14) */
        4387,/* a(15) */
        4615,/* a(16) */
        5916,/* a(17) */
        7791,/* a(18) */
        9940 /* a(19) */
    };

    if (n <= 19)
        return init[n];

    /* Static cache so repeated calls are cheap */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 19;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_size + 1));
        for (idx_t i = 1; i <= cache_size; ++i)
            cache[i] = init[i];
    }

    if (n > cache_size) {
        idx_t new_size = n;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            /* Use recurrence: a(i) = a(i-1) + 6*a(i-9) - 6*a(i-10) - a(i-18) + a(i-19) */
            seq_t term = cache[i - 1]
                       + cache[i - 9] * (seq_t)6
                       + cache[i - 19];
            term -= cache[i - 10] * (seq_t)6;
            term -= cache[i - 18];
            cache[i] = term;
        }
        cache_size = new_size;
    }

    return cache[n];
}
