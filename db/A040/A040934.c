/*
 * A040934  Continued fraction for sqrt(966).
 * https://oeis.org/A040934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = nth term of the continued fraction expansion of √966 a(0)
 *            = floor(√966) = 31; the periodic part repeats after this. The
 *            period is generated once on the first call and is reused for all
 *            subsequent calls
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Radicand and its integer part */
    const idx_t radicand = 966ULL;
    const idx_t a0        = 31ULL;   /* floor(√966) */

    /* Period cache */
    static bool          initialized = false;
    static seq_t         period[64];
    static size_t        period_len   = 0;

    if (!initialized) {
        /* Temporary storage while building the period */
        idx_t m      = 0;
        idx_t d      = 1;
        idx_t a_val  = a0;
        idx_t a_next;
        size_t i = 0;

        /* Build the periodic part until the term 2*a0 appears */
        do {
            idx_t m_next = d * a_val - m;
            idx_t d_next = (radicand - m_next * m_next) / d;
            a_next       = (a0 + m_next) / d_next;

            period[i] = (seq_t)a_next;
            ++i;

            m      = m_next;
            d      = d_next;
            a_val  = a_next;
        } while (a_next != 2 * a0);

        period_len   = i;
        initialized = true;
    }

    if (n == 0)
        return (seq_t)a0;

    /* (n‑1) because a(1) is the first periodic term */
    size_t idx = (size_t)((n - 1) % (idx_t)period_len);
    return period[idx];
}
