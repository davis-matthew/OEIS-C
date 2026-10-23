/*
 * A010203  Continued fraction for sqrt(151).
 * https://oeis.org/A010203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(151) */
/* a(0) = floor(sqrt(151)) = 12.                  */
/* For n ≥ 1, a(n) are the periodic terms of the continued fraction of sqrt(151). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache for the period a(1)..a(p). */
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Compute the period once. */
        const seq_t D  = 151;   /* D in sqrt(D) */
        const seq_t a0 = 12;    /* floor(sqrt(D)) */

        /* Working variables for the recurrence. */
        seq_t m        = 0;     /* m_0 */
        seq_t d        = 1;     /* d_0 */
        seq_t a_prev   = a0;    /* a_0 */

        /* Temporary buffer for period entries. */
        size_t cap = 16;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!period) {
            /* Allocation failed – cannot proceed. */
            return 0;
        }
        size_t count = 0;

        for (;;) {
            /* Recurrence: m_{k+1} = d_k * a_k - m_k */
            seq_t m_next = d * a_prev - m;

            /* d_{k+1} = (D - m_{k+1}^2) / d_k */
            seq_t d_next = (D - m_next * m_next) / d;

            /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
            seq_t a_next = (a0 + m_next) / d_next;

            /* Resize buffer if necessary. */
            if (count == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
                if (!tmp) {
                    free(period);
                    period = NULL;
                    return 0;
                }
                period = tmp;
            }

            period[count++] = a_next;

            /* The period ends when the next d becomes 1.
               The term a_next (24 for sqrt(151)) is part of the period. */
            if (d_next == 1)
                break;

            /* Advance to next step. */
            m      = m_next;
            d      = d_next;
            a_prev = a_next;
        }

        period_len = count;
        initialized = true;
    }

    /* Handle the first term explicitly. */
    if (n == 0U)
        return 12;  /* a(0) = floor(sqrt(151)) */

    /* For n ≥ 1 use periodic part. */
    if (period_len == 0)
        return 0;   /* Should not happen, but guard against division by zero. */

    size_t idx = (size_t)((n - 1U) % period_len);
    return period[idx];
}
