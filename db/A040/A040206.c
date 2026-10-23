/*
 * A040206  Continued fraction for sqrt(221).
 * https://oeis.org/A040206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(221)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*
 * a(n) is the n-th term of the simple continued fraction expansion of
 * sqrt(221), with a(0) = floor(sqrt(221)) = 14.
 * The period after the first term is 1,6,2,6,1,28.
 */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t period[256];
    static idx_t period_len;
    static seq_t a0;

    if (!init) {
        /* compute a0 = floor(sqrt(221)) */
        a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= (seq_t)221)
            a0++;

        /* compute the periodic part of the continued fraction */
        seq_t m = 0;        /* previous m value  */
        seq_t d = 1;        /* previous d value  */
        seq_t cur = a0;     /* current a value   */
        period_len = 0;

        for (;;) {
            seq_t m_next = d * cur - m;
            seq_t diff   = (seq_t)221 - m_next * m_next;
            seq_t d_next = diff / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = a_next;

            /* In the continued‑fraction expansion of sqrt(N),
             * the period ends when the term equals 2*a0.          */
            if (a_next == 2 * a0)
                break;

            m = m_next;
            d = d_next;
            cur = a_next;
        }

        init = true;
    }

    if (n == 0)
        return a0;

    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
