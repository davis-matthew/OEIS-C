/*
 * A105262  a(n)=number of tilings of a 4 X n rectangle using tiles that are
 *          either 1 X 1 squares or trominoes (here by a tromino we mean a 2 X
 *          2 square with the upper right 1 X 1 square removed; no rotations
 *          allowed).
 * https://oeis.org/A105262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+5*a(n-2)+4*a(n-3)-a(n-5),
 *            a(0)=1,a(1)=1,a(2)=5,a(3)=13,a(4)=42
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache has the first five values */
    if (cache_size < 5) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * 5);
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)5;   /* a(2) */
        cache[3] = (seq_t)13;  /* a(3) */
        cache[4] = (seq_t)42;  /* a(4) */
        cache_size = 5;
    }

    /* if requested term already cached, return it immediately */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend cache to include requested index */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence valid for i >= 5 */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a5 = cache[i - 5];
        cache[i] = a1 + (seq_t)5 * a2 + (seq_t)4 * a3 - a5;
    }
    cache_size = n + 1;

    return cache[n];
}
