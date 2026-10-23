/*
 * A040538  Continued fraction for sqrt(562).
 * https://oeis.org/A040538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction coefficients for sqrt(562)
    a(0) = floor(sqrt(562)) = 23
    a(n) for n>0 are the periodic part of the continued fraction.
    The period ends when a coefficient equals 2*floor(sqrt(562)). */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

seq_t a(idx_t n)
{
    /* Static cache of the continued‑fraction coefficients. */
    static bool initialized = false;
    static idx_t a0 = 0;                 /* floor(sqrt(562))   */
    static size_t period_len = 0;        /* length of the period */
    static idx_t period[128];            /* periodic coefficients */

    if (!initialized) {
        /* Compute a0 = floor(sqrt(562)) as an unsigned integer. */
        const idx_t S = 562;
        idx_t x;
        for (x = 1; x * x <= S; ++x) {
            a0 = x;
        }

        /* Compute the periodic part of the continued fraction. */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_cur = a0;   /* first a0 is already known */

        period_len = 0;
        while (true) {
            /* Next state equations (all on unsigned integers). */
            idx_t m_next   = d * a_cur - m;
            idx_t d_next   = (S - m_next * m_next) / d;
            idx_t a_next   = (a0 + m_next) / d_next;

            /* Store the coefficient in the period array. */
            if (period_len >= sizeof(period)/sizeof(period[0])) {
                /* Should never happen for sqrt(562). */
                break;
            }
            period[period_len++] = a_next;

            /* The period for sqrt(N) ends with the first term 2*a0. */
            if (a_next == 2 * a0) {
                break;
            }

            /* Advance to the next state. */
            m     = m_next;
            d     = d_next;
            a_cur = a_next;
        }

        initialized = true;
    }

    if (n == 0) {
        return (seq_t)a0;
    }

    /* For n > 0, the terms repeat with the period. */
    size_t idx = (size_t)((n - 1) % period_len);
    return (seq_t)period[idx];
}
