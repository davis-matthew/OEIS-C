/*
 * A215868  Number of permutations of 0..floor((n*8-2)/2) on odd squares of an
 *          nX8 array such that each row, column, diagonal and (downwards)
 *          antidiagonal of odd squares is increasing.
 * https://oeis.org/A215868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 272*a(n-2) - 3439*a(n-4) - 3336*a(n-6) + 140*a(n-8) with
 *            initial values: a(1)=1, a(2)=14, a(3)=110, a(4)=3001,
 *            a(5)=26451, a(6)=767560, a(7)=6812794, a(8)=198409297
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;

    if (cache == NULL) {
        cache_size = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    }
    if (n + 1 > cache_size) {
        idx_t new_size = (n + 1) * 2;
        if (new_size < 16) new_size = 16;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 1; i <= computed_max; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    if (computed_max < 8) {
        static const seq_t base[9] = {
            0,        /* unused */
            1,        /* a(1) */
            14,       /* a(2) */
            110,      /* a(3) */
            3001,     /* a(4) */
            26451,    /* a(5) */
            767560,   /* a(6) */
            6812794,  /* a(7) */
            198409297 /* a(8) */
        };
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = base[i];
        computed_max = 8;
    }

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i >= 9) {
            seq_t t = 272 * cache[i-2];
            t -= 3439 * cache[i-4];
            t -= 3336 * cache[i-6];
            t += 140 * cache[i-8];
            cache[i] = t;
        }   /* else values <= 8 already set */
    }
    computed_max = n;

    return cache[n];
}
