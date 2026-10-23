/*
 * A010213  Continued fraction for sqrt(165).
 * https://oeis.org/A010213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(165)
   The expansion of √165 starts 12, [1,5,2,5,1,24] repeating.
   a(0) = 12,  a(1) = 1,  a(2) = 5, …  */

#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t a0 = 12;          /* floor(√165)   */
    const idx_t D  = 165;         /* radicand      */

    /* Periodic part after the first term – stored on first call. */
    static seq_t period[12];
    static size_t period_len = 0;
    static int    initialized = 0;

    if (!initialized) {
        /* Classic continued‑fraction iteration for square roots. */
        idx_t m     = 0;
        idx_t d     = 1;
        idx_t aPrev = a0;
        size_t i    = 0;

        while (1) {
            idx_t m_next = d * aPrev - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            period[i++] = (seq_t)a_next;

            /* √D periodic when the next term equals 2*a0. */
            if (a_next == 2 * a0)
                break;

            m     = m_next;
            d     = d_next;
            aPrev = a_next;
        }

        period_len   = i;
        initialized  = 1;
    }

    if (n == 0)
        return (seq_t)a0;

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
