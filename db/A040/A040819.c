/*
 * A040819  Continued fraction for sqrt(849).
 * https://oeis.org/A040819
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

/* Continued fraction for sqrt(849): a(0)=floor(sqrt(849)), afterwards the
   periodic part of its continued fraction expansion for a quadratic irrational. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t D = (seq_t)849;
    static seq_t a0 = 0;          // floor(sqrt(D))
    static seq_t *period = NULL;  // periodic part (a(1), a(2), ...)
    static size_t period_len = 0;
    static idx_t initialized = 0;

    /* Initialise once */
    if (!initialized) {
        /* Compute a0 */
        seq_t m = 0;
        while ((m + 1) * (m + 1) <= D)
            m++;
        a0 = m;

        /* Build the periodic part */
        // Allocate an initial small buffer; will grow if needed
        size_t cap = 16;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!period)
            return 0;  // memory failure fallback

        seq_t m_k = 0;
        seq_t d_k = 1;
        seq_t a_k = a0;          // a_k is the term used to compute the next m,d
        size_t pos = 0;

        for (;;) {
            /* compute next m,d,a */
            seq_t m_next = d_k * a_k - m_k;
            seq_t d_next = (D - m_next * m_next) / d_k;
            seq_t a_next = (a0 + m_next) / d_next;

            /* store the period term (a_next corresponds to a(n) for n>=1) */
            if (pos == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
                if (!tmp) {
                    free(period);
                    return 0;  // memory failure fallback
                }
                period = tmp;
            }
            period[pos++] = a_next;

            /* stop when the period repeats: d_next == 1 */
            if (d_next == 1)
                break;

            /* advance to next state */
            m_k = m_next;
            d_k = d_next;
            a_k = a_next;
        }
        period_len = pos;
        initialized = 1;
    }

    if (n == 0)
        return a0;

    /* a(1) is period[0] */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
