/*
 * A040611  Continued fraction for sqrt(637).
 * https://oeis.org/A040611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A040611: continued‑fraction terms of sqrt(637).
 *  a(0) = ⌊√637⌋, a(1) = ⌊(a0 + m1)/d1⌋, ... .
 *  The periodic part is generated once and reused for all calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t); */

seq_t a(idx_t n)
{
    /* static cache of the period terms: coeff[0] = a0, coeff[1..P] are the period */
    static seq_t *coeff = NULL;
    static size_t period = 0;      /* number of period terms after a0 (P) */
    static bool init_done = false;

    if (!init_done) {
        const seq_t D = (seq_t)637;
        const seq_t a0 = floor_sqrt(D);

        /* allocate buffer for the period (do not know how many terms yet) */
        size_t cap = 64;                           /* ample for D = 637 */
        coeff = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!coeff) return 0;                      /* very unlikely */
        coeff[0] = a0;

        /* variables for generating the continued fraction */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_term = a0;                      /* a0 used in recurrence */

        size_t idx = 1;                         /* next position (a1) */

        while (1) {
            /* m_k = d_{k-1} * a_{k-1} - m_{k-1} */
            m = d * a_term - m;

            /* d_k = (D - m_k^2) / d_{k-1} */
            d = (D - m * m) / d;

            /* a_k = ⌊(a0 + m_k)/d_k⌋ */
            a_term = (a0 + m) / d;

            /* store the term a_k */
            if (idx >= cap - 1) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(coeff, cap * sizeof(seq_t));
                if (!tmp) { free(coeff); coeff = NULL; return 0; }
                coeff = tmp;
            }
            coeff[idx] = a_term;
            idx++;

            /* period finishes when a_k == 2*a0 (standard for sqrt(D)) */
            if (a_term == 2 * a0) {
                period = idx - 1;                /* number of terms after a0 */
                /* shrink buffer to exactly period+1 terms */
                seq_t *tmp = (seq_t *)realloc(coeff, (period + 1) * sizeof(seq_t));
                if (tmp) coeff = tmp;
                break;
            }
        }
        init_done = true;
    }

    if (n == 0) return coeff[0];

    if (period == 0) return 0;                     /* should not happen */
    idx_t r = (n - 1) % period;                    /* 0 <= r < period */
    return coeff[1 + r];                           /* coeff[1] is a1 */
}
