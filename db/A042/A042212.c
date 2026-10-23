/*
 * A042212  Numerators of continued fraction convergents to sqrt(632).
 * https://oeis.org/A042212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 15486 * a(n-4) - a(n-8) Initial values: a(0) = 25, a(1) =
 *            176, a(2) = 1081, a(3) = 7743, a(4) = 388231, a(5) = 2725360,
 *            a(6) = 16740391, a(7) = 119908097
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t computed_end = 8;   /* smallest n for which all needed terms exist */

    /* Allocate and initialize cache if necessary */
    if (!cache) {
        cap = 8;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)25;
        cache[1] = (seq_t)176;
        cache[2] = (seq_t)1081;
        cache[3] = (seq_t)7743;
        cache[4] = (seq_t)388231;
        cache[5] = (seq_t)2725360;
        cache[6] = (seq_t)16740391;
        cache[7] = (seq_t)119908097;
    }

    /* Grow cache if n+1 exceeds current capacity */
    if (n + 1 > cap) {
        size_t newcap = n + 1;
        if (newcap < cap * 2) newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed_end; i <= n; ++i) {
        seq_t term = cache[i - 4] * (seq_t)15486 - cache[i - 8];
        cache[i] = term;
    }
    if (n + 1 > computed_end) computed_end = n + 1;

    return cache[n];
}
