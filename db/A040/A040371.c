/*
 * A040371  Continued fraction for sqrt(391).
 * https://oeis.org/A040371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(391) obtained by the standard
 *            algorithm for quadratic irrationals. The sequence starts with a0
 *            = floor(sqrt(391)) = 19 and has a purely periodic part of length
 *            16. The periodic part is [1,3,2,2,1,1,2,19,2,1,1,2,2,3,1,38]
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
    /* static cache for the periodic part */
    static bool init = false;
    static seq_t period[64];
    static size_t period_len = 0U;

    if (!init) {
        const seq_t D  = 391;      /* the radicand */
        const seq_t a0 = 19;       /* floor(sqrt(391)) */
        seq_t m = 0, d = 1, a_prev = a0;
        size_t idx = 0;

        /* Generate the periodic part until a_i == 2*a0 (38). */
        while (1) {
            seq_t m_next = d * a_prev - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_curr = (a0 + m_next) / d_next;

            period[idx++] = a_curr;

            /* End of period: the last term equals 2*a0. */
            if (a_curr == 2 * a0) {
                period_len = idx;
                break;
            }
            /* Prepare for next iteration */
            m = m_next;
            d = d_next;
            a_prev = a_curr;
        }
        init = true;
    }

    /* a(0) is the first term, not part of the periodic part. */
    if (n == 0)
        return 19;

    /* Use the only periodic part for all n > 0. */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
