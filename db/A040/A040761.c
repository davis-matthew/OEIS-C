/*
 * A040761  Continued fraction for sqrt(790).
 * https://oeis.org/A040761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms for sqrt(790) computed by the standard
 *            algorithm for quadratic irrationals
 * Range:     u8 none, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cf = NULL;
    static size_t cf_cap = 0;
    static size_t cf_len = 0;      /* number of terms stored in cf */

    /* state for generating subsequent terms */
    static seq_t m = 0;            /* multiplier term */
    static seq_t d = 1;            /* divisor term */
    static seq_t prev_a = 0;       /* a(k‑1) */
    static seq_t a_start = 0;      /* floor(sqrt(790)) */
    static const seq_t D = (seq_t)790;

    /* initialize cache and state on first call */
    if (cf == NULL) {
        a_start = (seq_t)floor_sqrt(D);
        prev_a = a_start;

        cf_cap = 16;
        cf = (seq_t *)malloc(cf_cap * sizeof(seq_t));
        if (cf == NULL) return 0;          /* allocation failure – unlikely */
        cf[0] = a_start;
        cf_len = 1;
    }

    /* compute terms up to n if necessary */
    while (cf_len <= (size_t)n) {

        /* standard continued‑fraction recurrence */
        m = d * prev_a - m;
        d = (D - m * m) / d;
        seq_t next_a = (a_start + m) / d;

        /* grow cache if needed */
        if (cf_len == cf_cap) {
            size_t new_cap = cf_cap * 2;
            if (new_cap == 0) new_cap = 16;
            seq_t *tmp = (seq_t *)realloc(cf, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;    /* allocation failure – unlikely */
            cf = tmp;
            cf_cap = new_cap;
        }

        cf[cf_len++] = next_a;
        prev_a = next_a;
    }

    return cf[(size_t)n];
}
