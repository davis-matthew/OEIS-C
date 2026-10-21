/*
 * A221887  Number of nX3 arrays of occupancy after each element moves to some
 *          horizontal, diagonal or antidiagonal neighbor, without 2-loops or
 *          left turns.
 * https://oeis.org/A221887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-2) + 28*a(n-3) + a(n-4) - 104*a(n-5) - 92*a(n-6) +
 *            84*a(n-7) + 19*a(n-8) - 4*a(n-9) + 75*a(n-10) - 15*a(n-12) -
 *            8*a(n-13) + 4*a(n-14) Initial values: a(1)=0, a(2)=8, a(3)=0,
 *            a(4)=90, a(5)=204, a(6)=1297, a(7)=4608, a(8)=23159, a(9)=92312,
 *            a(10)=425249, a(11)=1799292, a(12)=7982268,...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;            /* number of entries cached (indices 0..size-1) */

    if (n < 1) return 0;              /* out of valid range */

    /* enlarge cache if needed */
    if (size <= n) {
        idx_t new_size = n + 1;       /* need indices 0..n inclusive */
        size_t bytes = (size_t)new_size * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, bytes);
        if (!tmp) abort();           /* allocation failed */
        cache = tmp;

        /* base values (indices 1..15) */
        static const seq_t base[16] = {
            0,          /* a(0) placeholder */
            0,          /* a(1) */
            8,          /* a(2) */
            0,          /* a(3) */
            90,         /* a(4) */
            204,        /* a(5) */
            1297,       /* a(6) */
            4608,       /* a(7) */
            23159,      /* a(8) */
            92312,      /* a(9) */
            425249,     /* a(10) */
            1799292,    /* a(11) */
            7982268,    /* a(12) */
            34469356,   /* a(13) */
            151243634,  /* a(14) */
            657198136   /* a(15) */
        };

        for (idx_t i = size; i <= n; ++i) {
            if (i <= 15) {
                cache[i] = base[i];
            } else {
                /* recurrence for i >= 16 */
                seq_t val =
                    (seq_t)14 * cache[i-2] +
                    (seq_t)28 * cache[i-3] +
                      cache[i-4] -
                    (seq_t)104 * cache[i-5] -
                    (seq_t)92  * cache[i-6] +
                    (seq_t)84  * cache[i-7] +
                    (seq_t)19  * cache[i-8] -
                    (seq_t)4   * cache[i-9] +
                    (seq_t)75  * cache[i-10] -
                    (seq_t)15  * cache[i-12] -
                    (seq_t)8   * cache[i-13] +
                    (seq_t)4   * cache[i-14];
                cache[i] = val;
            }
        }

        size = new_size;
    }

    return cache[n];
}
