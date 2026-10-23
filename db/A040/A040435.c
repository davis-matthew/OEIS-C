/*
 * A040435  Continued fraction for sqrt(457).
 * https://oeis.org/A040435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = the n‑th partial quotient in the continued fraction
 *            expansion of sqrt(457) (a(0)=floor(sqrt(457))=21)
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[512];
    static size_t period_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Prepare the continued fraction for sqrt(457). */
        const seq_t D     = (seq_t)457;
        const seq_t a0    = (seq_t)21;     /* floor(sqrt(457)) */
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_curr = a0;                /* a in the recurrence */

        /* Generate the repeating block (starting with a1). */
        while (true) {
            seq_t m_next = d * a_curr - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;
            period[period_len++] = a_next;   /* store a_next (a1, a2, …) */
            if (a_next == a0 * (seq_t)2) {
                /* a_next == 2*a0 marks the end of the period. */
                break;
            }
            m = m_next;
            d = d_next;
            a_curr = a_next;
        }

        initialized = true;
    }

    if (n == (idx_t)0)
        return (seq_t)21;           /* a0 = 21 */

    /* a(n) for n >= 1 is period[(n-1) % period_len] */
    size_t idx = (size_t)((n - (idx_t)1) % period_len);
    return period[idx];
}
