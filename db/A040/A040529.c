/*
 * A040529  Continued fraction for sqrt(553).
 * https://oeis.org/A040529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = nth term in the continued fraction expansion of sqrt(553)
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;
    /* variables for continued fraction generation */
    static seq_t a0 = 0;     /* floor(sqrt(553)) */
    static seq_t last_m = 0;
    static seq_t last_d = 1;
    static seq_t last_a = 0; /* a_k */

    /* initialize on first call */
    if (len == 0) {
        a0 = floor_sqrt((seq_t)553u);
        cache = (seq_t *)malloc(sizeof(seq_t) * 32);
        if (!cache) return 0; /* fall back */
        cap = 32;
        cache[0] = a0;
        len = 1;
        last_m = 0;
        last_d = 1;
        last_a = a0;
    }

    /* extend cache until it contains index n */
    while (len <= n) {
        /* compute next term of the continued fraction */
        seq_t m_next = last_d * last_a - last_m;
        seq_t d_next = ( (seq_t)553u - m_next * m_next ) / last_d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* ensure capacity */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        cache[len] = a_next;
        ++len;

        /* update for next iteration */
        last_m = m_next;
        last_d = d_next;
        last_a = a_next;
    }

    return cache[n];
}
