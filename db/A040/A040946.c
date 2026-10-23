/*
 * A040946  Continued fraction for sqrt(978).
 * https://oeis.org/A040946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The terms a(n) are the coefficients of the continued fraction
   expansion of sqrt(978).  a(0) = floor(sqrt(978)), and the
   subsequent coefficients form a periodic sequence. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* √978 */
    const idx_t DI = 978;

    /* Static data used to generate the continued fraction once. */
    static int initialized = 0;
    static seq_t a0 = 0;            /* floor(sqrt(978)) */
    static seq_t *period = NULL;    /* array holding one full period */
    static size_t period_len = 0;   /* length of the period */

    if (!initialized)
    {
        /* Compute a0 = floor(sqrt(978)) */
        idx_t m = 0;
        while ((m + 1) * (m + 1) <= DI)
            ++m;
        a0 = (seq_t)m;

        /* Build the period by the standard algorithm for quadratic
           surds.  The period ends when a terminates with 2*a0. */
        idx_t m_k = 0;
        idx_t d_k = 1;
        seq_t a_k = a0;

        size_t cap = 32;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!period)          /* allocation failure -> return 0 */
            return 0;

        while (1)
        {
            idx_t m_next = d_k * (idx_t)a_k - m_k;
            idx_t d_next = (DI - m_next * m_next) / d_k;
            seq_t a_next = (a0 + (seq_t)m_next) / (seq_t)d_next;

            if (period_len == cap)
            {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
                if (!tmp)      /* allocation failure -> return 0 */
                    return 0;
                period = tmp;
            }
            period[period_len++] = a_next;

            if (a_next == 2 * a0)
                break;        /* end of the period */

            m_k = m_next;
            d_k = d_next;
            a_k = a_next;
        }

        initialized = 1;
    }

    if (n == 0)
        return a0;
    /* For n>0 the coefficients repeat with the computed period. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
