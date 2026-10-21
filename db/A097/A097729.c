/*
 * A097729  Pell equation solutions (6*a(n))^2 - 37*b(n)^2 = -1 with
 *          b(n):=A097730(n), n >= 0.
 * https://oeis.org/A097729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 146*a(n-1) - a(n-2) (n ≥ 2) with a(0) = 1, a(1) = 147
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* ensure the cache is large enough for indices up to n */
    if (cached <= n) {
        size_t newsize = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(newsize * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        }
        /* initialise base cases if not already done */
        if (cached == 0) {
            cache[0] = (seq_t)1;
            if (newsize > 1) {
                cache[1] = (seq_t)147;
                cached = 2;
            } else {
                cached = 1;
            }
        } else if (cached == 1 && newsize > 1) {
            cache[1] = (seq_t)147;
            cached = 2;
        }
        /* fill the rest using the recurrence */
        for (size_t i = cached; i < newsize; ++i) {
            seq_t prod = cache[i-1] * (seq_t)146;
            cache[i] = prod - cache[i-2];
        }
        cached = newsize;
    }
    return cache[(size_t)n];
}
