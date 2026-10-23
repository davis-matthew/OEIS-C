/*
 * A199561  a(n) = 3*9^n + 1.
 * https://oeis.org/A199561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*9^n + 1 Recurrence: a(0) = 4, a(n) = 9*a(n-1) - 8 (n ≥
 *            1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;              /* current allocated capacity           */

    /* Ensure the cache is large enough for index n */
    if (n >= (idx_t)cap) {
        size_t need = (size_t)n + 1;    /* need at least n+1 elements           */
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= need) new_cap <<= 1; /* grow by factors of two */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();

        /* Compute new entries, starting from the old capacity */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0) {
                tmp[0] = (seq_t)4;      /* a(0) = 4 */
            } else {
                /* a(i) = 9*a(i-1) - 8 */
                tmp[i] = tmp[i - 1] * (seq_t)9 - (seq_t)8;
            }
        }

        cache = tmp;
        cap = new_cap;
    }

    return cache[(size_t)n];
}
