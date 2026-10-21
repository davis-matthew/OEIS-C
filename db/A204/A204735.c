/*
 * A204735  Number of (n+1)X4 0..1 arrays with rows and columns of permanents
 *          of all 2X2 subblocks lexicographically nondecreasing, and all 2X2
 *          permanents nonzero.
 * https://oeis.org/A204735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+5*a(n-2)-30*a(n-3)-3*a(n-4)+88*a(n-5)-25*a(n-6)-126*a(n-7)+60*a(n-8)+88*a(n-9)-52*a(n-10)-24*a(n-11)+16*a(n-12)
 *            for n ≥ 13, with initial values: a(1)=31, a(2)=115, a(3)=409,
 *            a(4)=1389, a(5)=4658, a(6)=15327, a(7)=50177, a(8)=163232,
 *            a(9)=530016, a(10)=1717764, a(11)=5563837, a(12)...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* initialise static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initial 12 terms */
    static const seq_t init[12] = {
        31,      /* a(1) */
        115,     /* a(2) */
        409,     /* a(3) */
        1389,    /* a(4) */
        4658,    /* a(5) */
        15327,   /* a(6) */
        50177,   /* a(7) */
        163232,  /* a(8) */
        530016,  /* a(9) */
        1717764, /* a(10) */
        5563837, /* a(11) */
        18012221 /* a(12) */
    };

    if (n <= 12) {
        return init[n - 1];
    }

    /* grow cache if necessary */
    if (cache_size < n) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort */
            return 0;
        }
        cache = tmp;

        /* warm new slots */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 12) {
                cache[i] = init[i];
                continue;
            }

            /* recurrence */
            seq_t pos = 0;
            pos += cache[i - 1] * 4;
            pos += cache[i - 2] * 5;
            pos += cache[i - 5] * 88;
            pos += cache[i - 8] * 60;
            pos += cache[i - 9] * 88;
            pos += cache[i -12] * 16;

            seq_t neg = 0;
            neg += cache[i - 3] * 30;
            neg += cache[i - 4] * 3;
            neg += cache[i - 6] * 25;
            neg += cache[i - 7] * 126;
            neg += cache[i -10] * 52;
            neg += cache[i -11] * 24;

            cache[i] = pos - neg;  /* pos ≥ neg for all n ≥ 13 */
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
