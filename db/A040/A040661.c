/*
 * A040661  Continued fraction for sqrt(688).
 * https://oeis.org/A040661
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

/* Continued fraction for sqrt(688) – essentially a(0)=26 and the
   sequence repeats the 12‑term period:
   4, 2, 1, 5, 7, 3, 7, 5, 1, 2, 4, 52.
   The period is computed on first use and cached for subsequent calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the periodic part (a1 … a_last) */
    static seq_t *period = NULL;  /* array of period terms */
    static size_t period_len = 0;
    /* a0 is constant floor(sqrt(688)) */
    static const seq_t a0 = (seq_t)26;

    /* initialize period only once */
    if (!period) {
        /* temp buffer for generating the period */
        seq_t buffer[64];
        seq_t m = 0, d = 1, a_cur = a0;
        const seq_t two_a0 = a0 * 2;
        size_t i = 0;
        for (;;) {
            /* next values in the continued‑fraction algorithm */
            seq_t m_next = d * a_cur - m;
            seq_t d_next = ( ( (seq_t)688 ) - m_next * m_next ) / d;
            seq_t a_next = (a0 + m_next) / d_next;  /* integer division = floor */

            buffer[i++] = a_next;
            if (a_next == two_a0)
                break;   /* end of period */
            /* advance state */
            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
        period_len = i;

        /* allocate exact storage and copy the period */
        period = (seq_t *)malloc(period_len * sizeof(seq_t));
        if (!period) abort();
        for (i = 0; i < period_len; ++i)
            period[i] = buffer[i];
    }

    if (n == 0)
        return a0;
    /* return the n‑th term (n ≥ 1) */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
