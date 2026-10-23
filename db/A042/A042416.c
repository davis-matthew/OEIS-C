/*
 * A042416  Numerators of continued fraction convergents to sqrt(736).
 * https://oeis.org/A042416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 48670*a(n-8) - a(n-16) Initial values (for n = 0 … 15):
 *            a(0)=27, a(1)=190, a(2)=217, a(3)=624, a(4)=841, a(5)=2306,
 *            a(6)=3147, a(7)=24335, a(8)=1317237, a(9)=9244994,
 *            a(10)=10562231, a(11)=30369456, a(12)=40931687, a(13)=112232830,
 *            a(14)=153164517, a(15)=1184384449
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache    = NULL;
    static size_t cache_sz = 0;   /* allocated size          */
    static size_t next_idx = 0;   /* first index not yet set */

    /* Ensure we have space for indices 0 … n. */
    size_t need = (size_t)n + 1;
    if (need < 16) need = 16;          /* reserve room for the 16 initial terms */
    if (!cache || need > cache_sz) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache     = tmp;
        cache_sz  = need;
    }

    /* Insert the 16 known initial terms once. */
    if (next_idx < 16) {
        static const seq_t init[16] = {
            27, 190, 217, 624, 841,
            2306, 3147, 24335, 1317237,
            9244994, 10562231, 30369456,
            40931687, 112232830, 153164517,
            1184384449
        };
        size_t i;
        for (i = next_idx; i < 16 && i < cache_sz; ++i)
            cache[i] = init[i];
        next_idx = 16;
    }

    /* Compute any terms up to and including n. */
    if (next_idx <= (size_t)n) {
        size_t i;
        for (i = next_idx; i <= (size_t)n; ++i) {
            /* a(i) = 48670*a(i-8) - a(i-16)   */
            seq_t prod = (seq_t)48670 * cache[i - 8];
            cache[i] = prod - cache[i - 16];
        }
        next_idx = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
