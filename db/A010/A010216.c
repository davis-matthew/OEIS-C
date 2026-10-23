/*
 * A010216  Continued fraction for sqrt(172).
 * https://oeis.org/A010216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the continued fraction expansion of sqrt(172).
   The sequence repeats with period 16 after the first term. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* period length for sqrt(172) */
    const idx_t period_len = 16;
    /* a(0) */
    const seq_t a0 = 13;

    static seq_t period[16];
    static bool initialized = false;

    if (!initialized)
    {
        /* compute the period terms using the standard
           continued‑fraction algorithm for square roots */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;
        const seq_t S = 172;  /* value under the square root */

        for (idx_t i = 0; i < period_len; ++i)
        {
            /* next m and d */
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (S - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[i] = a_next;

            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
        initialized = true;
    }

    if (n == 0)
        return a0;

    return period[(n - 1) % period_len];
}
