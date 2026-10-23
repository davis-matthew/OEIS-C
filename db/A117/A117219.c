/*
 * A117219  Cubes divisible by their number of digits.
 * https://oeis.org/A117219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 409, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: cubes divisible by their number of digits.
   Computed by brute‑force search. */
#include <stdlib.h>
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* current allocation size */
    static idx_t filled = 0;        /* number of cached terms */
    static idx_t m = 0;             /* next cube index to test */

    /* initial allocation */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        filled = 0;
        m = 0;
        if (cache == NULL) return 0; /* fallback on allocation failure */
    }

    while (filled < n) {
        /* compute next cube */
        seq_t val = (seq_t)m * (seq_t)m * (seq_t)m;

        /* number of decimal digits */
        int d = digit_len(val);

        /* check divisibility */
        if (val % (seq_t)d == 0) {
            /* expand cache if necessary */
            if (filled == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (tmp == NULL) return 0; /* fallback on realloc failure */
                cache = tmp;
            }
            cache[filled++] = val;
        }

        ++m;
    }

    return cache[n - 1];
}
