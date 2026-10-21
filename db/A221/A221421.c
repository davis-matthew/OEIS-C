/*
 * A221421  Number of 3Xn arrays of occupancy after each element stays put or
 *          moves to some horizontal or antidiagonal neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for odd n; for even n>20 a(n) = 36*a(n-2) – 200*a(n-4)
 *            – 40*a(n-6) – 84*a(n-8) + 80*a(n-10) – 20*a(n-12) + 80*a(n-14)
 *            Initial values (even n ≤ 20): a(2)=8, a(4)=226, a(6)=6798,
 *            a(8)=198823, a(10)=5789134, a(12)=168353683, a(14)=4894400220,
 *            a(16)=142279944512, a(18)=4135993312056, a(20)=1...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[21] = {
    (seq_t)0,                   /* 0 (unused)           */
    (seq_t)0,                   /* a(1) = 0 (odd)       */
    (seq_t)8ULL,                /* a(2)                 */
    (seq_t)0,                   /* a(3) = 0 (odd)       */
    (seq_t)226ULL,              /* a(4)                 */
    (seq_t)0,                   /* a(5)                 */
    (seq_t)6798ULL,             /* a(6)                 */
    (seq_t)0,                   /* a(7)                 */
    (seq_t)198823ULL,           /* a(8)                 */
    (seq_t)0,                   /* a(9)                 */
    (seq_t)5789134ULL,          /* a(10)                */
    (seq_t)0,                   /* a(11)                */
    (seq_t)168353683ULL,        /* a(12)                */
    (seq_t)0,                   /* a(13)                */
    (seq_t)4894400220ULL,       /* a(14)                */
    (seq_t)0,                   /* a(15)                */
    (seq_t)142279944512ULL,     /* a(16)                */
    (seq_t)0,                   /* a(17)                */
    (seq_t)4135993312056ULL,    /* a(18)                */
    (seq_t)0,                   /* a(19)                */
    (seq_t)120230312311224ULL   /* a(20)                */
};

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for all i up to current size */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of slots allocated */

    if (n < 1) return (seq_t)0;     /* offset is 1 */

    /* ensure cache large enough to contain index n */
    if (cache == NULL) {
        /* initial allocation: at least up to 20 to store the base values */
        size_t init = (size_t)((n > 20) ? 20 : n);
        cache = (seq_t *)malloc((init + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache_size = init + 1;

        /* initialize base values and zeros */
        for (size_t i = 1; i <= init; ++i) {
            if (i % 2 == 1) {
                cache[i] = (seq_t)0;
            } else {
                cache[i] = (i <= 20) ? base[i] : (seq_t)0;
            }
        }

        /* compute recurrence for indices already in the cache */
        for (size_t i = 1; i <= init; ++i) {
            if (i % 2 == 1 || i <= 20) continue;
            seq_t a2   = cache[i - 2];
            seq_t a4   = cache[i - 4];
            seq_t a6   = cache[i - 6];
            seq_t a8   = cache[i - 8];
            seq_t a10  = cache[i - 10];
            seq_t a12  = cache[i - 12];
            seq_t a14  = cache[i - 14];
            seq_t pos = a2  * (seq_t)36ULL
                      + a10 * (seq_t)80ULL
                      + a14 * (seq_t)80ULL;
            seq_t neg = a4  * (seq_t)200ULL
                      + a6  * (seq_t)40ULL
                      + a8  * (seq_t)84ULL
                      + a12 * (seq_t)20ULL;
            cache[i] = (pos > neg) ? pos - neg : (seq_t)0;
        }
    }

    if ((size_t)n > cache_size - 1) {
        size_t old_size = cache_size;
        cache_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;

        for (size_t i = old_size; i <= n; ++i) {
            if (i % 2 == 1) {
                cache[i] = (seq_t)0;
            } else if (i <= 20) {
                cache[i] = base[i];
            } else {
                seq_t a2   = cache[i - 2];
                seq_t a4   = cache[i - 4];
                seq_t a6   = cache[i - 6];
                seq_t a8   = cache[i - 8];
                seq_t a10  = cache[i - 10];
                seq_t a12  = cache[i - 12];
                seq_t a14  = cache[i - 14];
                seq_t pos = a2  * (seq_t)36ULL
                          + a10 * (seq_t)80ULL
                          + a14 * (seq_t)80ULL;
                seq_t neg = a4  * (seq_t)200ULL
                          + a6  * (seq_t)40ULL
                          + a8  * (seq_t)84ULL
                          + a12 * (seq_t)20ULL;
                cache[i] = (pos > neg) ? pos - neg : (seq_t)0;
            }
        }
    }

    return cache[(size_t)n];
}
