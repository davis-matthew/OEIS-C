/*
 * A040432  Continued fraction for sqrt(454).
 * https://oeis.org/A040432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued–fraction coefficients of sqrt(454)
    a(0) = ⌊√454⌋
    For i ≥ 0:
        m_{i+1} = d_i * a_i – m_i
        d_{i+1} = (454 – m_{i+1}²) / d_i
        a_{i+1} = ⌊ (a_0 + m_{i+1}) / d_{i+1} ⌋
    The period ends when a_{i+1} = 2·a_0 = 42.
    The sequence thereafter repeats with this period.  */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

static seq_t sqrt454_a0 = 0;          /* ⌊√454⌋ , computed once      */
static seq_t period[256] = {0};       /* periodic part of the CF      */
static size_t period_len = 0;         /* length of the period         */

seq_t a(idx_t n)
{
    /* --- compute ⌊√454⌋ once ---------------------------------------- */
    if (sqrt454_a0 == 0)
        sqrt454_a0 = floor_sqrt((seq_t)454);

    /* --- compute the period once, for any n -------------------------- */
    if (period_len == 0) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = sqrt454_a0;            /* a₀ */
        seq_t target = sqrt454_a0 * (seq_t)2; /* 2·a₀ = 42 */
        seq_t m_next, d_next, a_next;

        /* generate until a_next == 2·a₀ (last term of period) */
        do {
            m_next = d * a_cur - m;
            d_next = ((seq_t)454 - m_next * m_next) / d;
            a_next = (sqrt454_a0 + m_next) / d_next;

            period[period_len++] = a_next;

            if (a_next == target)
                break;

            /* advance to next triple */
            m = m_next;
            d = d_next;
            a_cur = a_next;

        } while (period_len < sizeof(period)/sizeof(period[0]));
    }

    /* --- answer the requested index -------------------------------- */
    if (n == 0)
        return sqrt454_a0;
    else
        return period[(size_t)(n - 1) % period_len];
}
