/*
 * A235013  Number of (n+1) X (4+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + a(n-2) - 38*a(n-3) + 33*a(n-4) + 97*a(n-5) * -
 *            127*a(n-6) - 88*a(n-7) + 154*a(n-8) + 12*a(n-9) - 48*a(n-10) *
 *            for n ≥ 11, with initial values * a(1)=680, a(2)=1112,
 *            a(3)=1912, a(4)=3560, a(5)=6856, * a(6)=13912, a(7)=28968,
 *            a(8)=62600, a(9)=138360, a(10)=314376. * * This impleme...
 * Range:     u8 none, u16 n <= 8, u32 n <= 20, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   add_scaled
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/add_scaled.h"

static const seq_t init_vals[10] = {
    680,   1112,  1912,  3560,  6856,
    13912, 28968, 62600, 138360, 314376
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* behaviour below the offset is unused */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (!cache) {
        cache_size = 10;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache)
            exit(1);              /* out‑of‑memory */
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = init_vals[i - 1];
    }

    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* positive contributions */
            seq_t pos = cache[i - 2];                     /* a(n-2) */
            pos = add_scaled(pos, cache[i - 1], 5);        /* 5*a(n-1) */
            pos = add_scaled(pos, cache[i - 4], 33);       /* 33*a(n-4) */
            pos = add_scaled(pos, cache[i - 5], 97);       /* 97*a(n-5) */
            pos = add_scaled(pos, cache[i - 8], 154);      /* 154*a(n-8) */
            pos = add_scaled(pos, cache[i - 9], 12);       /* 12*a(n-9) */

            /* negative contributions */
            seq_t neg = 0;
            neg = add_scaled(neg, cache[i - 3], 38);       /* 38*a(n-3) */
            neg = add_scaled(neg, cache[i - 6], 127);      /* 127*a(n-6) */
            neg = add_scaled(neg, cache[i - 7], 88);       /* 88*a(n-7) */
            neg = add_scaled(neg, cache[i - 10], 48);      /* 48*a(n-10) */

            cache[i] = pos - neg;
        }
        cache_size = n;
    }

    return cache[n];
}
