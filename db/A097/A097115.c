/*
 * A097115  Expansion of (1 + 11*x - 90*x^2 - 1100*x^3)/(1 - 201*x^2 +
 *          10100*x^4).
 * https://oeis.org/A097115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 11, a(2) = 111, a(3) = 1111, a(n) = 201 *
 *            a(n-2) – 10100 * a(n-4) for n ≥ 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;    /* number of elements currently stored */

    /* ensure capacity for indices up to n */
    if (size <= n) {
        idx_t new_size = (n < 4) ? 4 : n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort with minimal safety */
            exit(1);
        }
        cache = tmp;

        /* initialise first four terms if necessary */
        if (size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)11;
            cache[2] = (seq_t)111;
            cache[3] = (seq_t)1111;
            size = 4;
        }

        /* compute missing terms up to n */
        for (idx_t i = size; i <= n; ++i) {
            if (i < 4) continue;                 /* already set */
            seq_t term =
                (seq_t)201 * cache[i-2]          /* 201 * a(i-2) */
                - (seq_t)10100 * cache[i-4];     /* 10100 * a(i-4) */
            cache[i] = term;
        }
        size = new_size;
    }

    return cache[n];
}
