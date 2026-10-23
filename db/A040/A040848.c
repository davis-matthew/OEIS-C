/*
 * A040848  Continued fraction for sqrt(878).
 * https://oeis.org/A040848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A040848: Continued fraction coefficients of sqrt(878).
 * a(0) = floor(sqrt(878)) = 29.
 * For n>=1, a(n) is the (n‑1)-th element of the periodic part of the
 * expansion; the period ends when d becomes 1 (the following coefficient
 * is 2*a(0)).
 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* constants for D = 878 */
    const seq_t D   = (seq_t)878;
    const seq_t a0  = (seq_t)29;

    /* initialization of the periodic part */
    static bool          init          = false;
    static seq_t *       period        = NULL;
    static size_t        period_len    = 0;

    if (n == 0)
        return a0;

    if (!init) {
        /* compute the period */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_curr = a0;

        /* growable array for period elements */
        size_t cap = 4;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        period_len = 0;

        while (1) {
            seq_t m_next = d * a_curr - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            /* store a_next */
            if (period_len == cap) {
                cap *= 2;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = a_next;

            /* stop when d becomes 1 (next coefficient is 2*a0) */
            if (d_next == 1)
                break;

            /* advance */
            m = m_next;
            d = d_next;
            a_curr = a_next;
        }
        init = true;
    }

    /* return the correct term */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
