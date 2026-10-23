/*
 * A040594  Continued fraction for sqrt(619).
 * https://oeis.org/A040594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th term in the continued‑fraction expansion of sqrt(619) */
/* Term 0 is floor(sqrt(619)) = 24, subsequent terms repeat with the period
   that ends when an term equals 2*floor(sqrt(619)). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The squarefree part D = 619 */
    static const idx_t D = 619;
    static seq_t a0 = 0;          /* floor(sqrt(D)) */
    static seq_t *period = NULL;  /* holds terms a1, a2, ... up to the period end */
    static idx_t period_len = 0;  /* number of period terms */
    static int initialized = 0;   /* flag for first call */

    if (!initialized) {
        /* compute a0 once */
        a0 = floor_sqrt((seq_t)D);

        /* Build the period */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;

        /* start with a capacity that will grow if needed */
        size_t cap = 16;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        period_len = 0;

        while (1) {
            /* compute next values */
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            /* store next term as part of the period */
            if (period_len == cap) {
                cap <<= 1;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = a_next;

            /* terminate when period completes */
            if (a_next == (seq_t)(2 * a0))
                break;

            /* advance */
            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
        initialized = 1;
    }

    if (n == 0)
        return a0;

    /* For n >= 1, index into the periodic part */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
