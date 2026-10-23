/*
 * A040246  Continued fraction for sqrt(263).
 * https://oeis.org/A040246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sequence: continued‑fraction coefficients of sqrt(263) */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t coeff[64];
    static idx_t period_len = 0;

    if (!initialized) {
        const seq_t D  = (seq_t)263;
        const seq_t a0 = (seq_t)16;

        seq_t m     = (seq_t)0;
        seq_t d     = (seq_t)1;
        seq_t a_cur = a0;

        coeff[0] = a_cur;
        idx_t i = 1;

        while (1) {
            /* Use a wider unsigned type for intermediates to avoid overflow. */
            unsigned int m_next = (unsigned int)d * (unsigned int)a_cur - (unsigned int)m;
            unsigned int d_next = ((unsigned int)D - m_next * m_next) / (unsigned int)d;
            unsigned int a_next = ((unsigned int)a0 + m_next) / d_next;

            coeff[i] = (seq_t)a_next;

            /* The period ends when a_k = 2*a0. */
            if (a_next == (unsigned int)(a0 << 1)) {
                period_len = i;
                break;
            }

            m     = (seq_t)m_next;
            d     = (seq_t)d_next;
            a_cur = (seq_t)a_next;
            ++i;
        }
        initialized = true;
    }

    if (n == 0)
        return coeff[0];

    idx_t idx = 1 + (n - 1) % period_len;
    return coeff[idx];
}
