/*
 * A042470  Numerators of continued fraction convergents to sqrt(763).
 * https://oeis.org/A042470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(763)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)27;
    const uint64_t D       = 763ULL;
    const uint64_t a0_val  = 27ULL;

    /* Continued fraction state for sqrt(763) */
    uint64_t m   = 0;          /* m_0        */
    uint64_t d   = 1;          /* d_0        */
    uint64_t a_cur = a0_val;   /* a_0        */

    /* Convergent numerator recurrence */
    seq_t Pm2 = 0;   /* p_{-2} */
    seq_t Pm1 = 1;   /* p_{-1} */
    seq_t P   = 0;   /* p_n   */

    /* i = 0  (first convergent) */
    P   = (seq_t)a_cur * Pm1 + Pm2;   /* p_0 = a0 * p_{-1} + p_{-2} */
    Pm2 = Pm1;
    Pm1 = P;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute next continued‑fraction coefficient */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0_val + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_cur = a_next;

        /* compute numerator of next convergent */
        P = (seq_t)a_cur * Pm1 + Pm2;

        if (i == n)
            return P;

        /* shift for next iteration */
        Pm2 = Pm1;
        Pm1 = P;
    }
    return P; /* unreachable but keeps compiler happy */
}
