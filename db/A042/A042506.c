/*
 * A042506  Numerators of continued fraction convergents to sqrt(781).
 * https://oeis.org/A042506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(781) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute the finite continued‑fraction period of sqrt(781). */
    const int D   = 781;
    const int A0  = 27;          /* floor(sqrt(781))    */
    const int AEND= 2*A0;        /* terminating a_k     */

    /* The period length is small (< 100), so a static array is fine. */
    int period[200];
    size_t period_len = 0;

    int m = 0, d = 1, a_prev = A0;
    while (1)
    {
        int m_next = d * a_prev - m;
        int d_next = (D - m_next * m_next) / d;
        int a_next = (A0 + m_next) / d_next;

        period[period_len++] = a_next;

        /* The period ends when the partial quotient equals 2*A0. */
        if (a_next == AEND)
            break;

        m      = m_next;
        d      = d_next;
        a_prev = a_next;
    }

    /* Compute the n‑th numerator by the standard recurrence. */
    if (n == 0)
        return (seq_t)A0;

    seq_t p_prev = (seq_t)1;   /* p_{-1} */
    seq_t p_curr = (seq_t)A0;  /* p_{0}  */

    for (idx_t i = 1; i <= n; ++i)
    {
        size_t idx = (size_t)((i - 1) % (idx_t)period_len);
        seq_t a_i  = (seq_t)period[idx];
        seq_t next = a_i * p_curr + p_prev;
        p_prev = p_curr;
        p_curr = next;
    }

    return p_curr;
}
