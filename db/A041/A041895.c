/*
 * A041895  Denominators of continued fraction convergents to sqrt(469).
 * https://oeis.org/A041895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 274430*a(n-10) - a(n-20) for n>19 Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=32, a(5)=195, a(6)=1982, a(7)=2177,
 *            a(8)=4159, a(9)=6336, a(10)=270271, a(11)=276607, a(12)=546878,
 *            a(13)=823485, a(14)=8781728, a(15)=53513853, a(16)=543920258,
 *            a(17)=597434111, a(18)=1141354369, a(19)=...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_values[20] = {
    (seq_t)1,      /* a(0) */
    (seq_t)1,      /* a(1) */
    (seq_t)2,      /* a(2) */
    (seq_t)3,      /* a(3) */
    (seq_t)32,     /* a(4) */
    (seq_t)195,    /* a(5) */
    (seq_t)1982,   /* a(6) */
    (seq_t)2177,   /* a(7) */
    (seq_t)4159,   /* a(8) */
    (seq_t)6336,   /* a(9) */
    (seq_t)270271, /* a(10) */
    (seq_t)276607, /* a(11) */
    (seq_t)546878, /* a(12) */
    (seq_t)823485, /* a(13) */
    (seq_t)8781728,/* a(14) */
    (seq_t)53513853,/* a(15) */
    (seq_t)543920258,/* a(16) */
    (seq_t)597434111,/* a(17) */
    (seq_t)1141354369,/* a(18) */
    (seq_t)1738788480 /* a(19) */
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of allocated slots */

    /* Initial allocation and seed with known values */
    if (cache == NULL) {
        size_t needed = (n + 1 > 20) ? (n + 1) : 20;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        for (idx_t i = 0; i < 20; ++i)
            cache[i] = init_values[i];
        cache_size = needed;
    }

    /* Grow cache if necessary and compute missing entries */
    if (n + 1 > cache_size) {
        idx_t old_size = cache_size;
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_size = n + 1;
        for (idx_t i = old_size; i <= n; ++i) {
            /* For i < 20 values are already set to init_values */
            if (i >= 20) {
                cache[i] = ((seq_t)274430) * cache[i - 10] - cache[i - 20];
            }
        }
    }

    return cache[n];
}
