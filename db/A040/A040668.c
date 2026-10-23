/*
 * A040668  Continued fraction for sqrt(695).
 * https://oeis.org/A040668
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

/* Continued fraction coefficients for sqrt(695).
   After the initial term a(0)=26 the period is [2,1,3,10,3,1,2,52]. */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    static seq_t a0 = 0;
    static seq_t period[16];
    static idx_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Compute a0 = floor(sqrt(695)) */
        a0 = floor_sqrt((seq_t)695);

        /* Generate the period using the standard continued‑fraction algorithm. */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_curr = a0;
        idx_t cnt = 0;
        seq_t a_next;
        do {
            seq_t m_next = d * a_curr - m;
            seq_t d_next = ( (seq_t)695 - m_next * m_next ) / d;
            a_next = (a0 + m_next) / d_next;

            period[cnt++] = a_next;   /* store coefficient */

            /* advance to next step */
            m = m_next;
            d = d_next;
            a_curr = a_next;
        } while (a_next != 2 * a0);   /* stop after the element 2*a0 (52) */

        period_len = cnt;
        initialized = 1;
    }

    if (n == 0) return a0;
    return period[(n - 1) % period_len];
}
