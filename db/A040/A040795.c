/*
 * A040795  Continued fraction for sqrt(824).
 * https://oeis.org/A040795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(824) using the standard algorithm.
   a(0) = floor(sqrt(824)) = 28.
   For n >= 1, the terms repeat with period 12:
   1, 2, 2, 1, 1, 6, 1, 1, 2, 2, 1, 56
   The function computes the period once and returns the requested
   term for any n (0-based indexing). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(824).  The period is known to be 12. */
    const seq_t D     = (seq_t)824;         /* D = 824          */
    const seq_t a0    = (seq_t)28;          /* floor(sqrt(824)) */
    const idx_t period_len = 12;

    /* Static storage for the period and a flag for one-time initialization. */
    static bool initialized = false;
    static seq_t period[12];

    /* Initialize the period on the first call. */
    if (!initialized) {
        /* Classic continued‑fraction generation for sqrt(D). */
        idx_t   m  = 0;          /* m_{k}   */
        idx_t   d  = 1;          /* d_{k}   */
        seq_t   a  = a0;         /* a_{k}   */
        idx_t   len = 0;         /* number of terms found so far */

        while (1) {
            seq_t m_next = (seq_t)d * a - (seq_t)m;                     /* m_{k+1} */
            seq_t d_next = ((seq_t)D - m_next * m_next) / (seq_t)d;       /* d_{k+1} */
            seq_t a_next = (a0 + m_next) / d_next;                      /* a_{k+1} */

            period[len++] = a_next;                                     /* store term */

            /* The period ends when a_{k+1} = 2*a0.  For D = 824, a0 = 28, so 2*a0 = 56. */
            if (a_next == a0 * (seq_t)2)
                break;

            /* Advance to next step. */
            m = (idx_t)m_next;
            d = (idx_t)d_next;
            a = a_next;
        }
        initialized = true;
    }

    /* Return the required term. */
    if (n == 0)
        return a0;                                   /* first term a(0) */
    else
        return period[(n - 1) % period_len];          /* periodic part */
}
