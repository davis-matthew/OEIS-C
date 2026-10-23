/*
 * A041444  Numerators of continued fraction convergents to sqrt(238).
 * https://oeis.org/A041444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the nth convergent of √238. Recurrence:
 *            p_{-2} = 0, p_{-1} = 1, a_0 = 15, for k ≥ 0: p_k = a_k * p_{k-1}
 *            + p_{k-2}, where a_k are the partial quotients of the continued
 *            fraction expansion of √238 (a_0 = 15, a_1 = 2, a_2 = 2, a_3 = 1,
 *            …)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t NUM = 238;      /* the radicand */
    const idx_t SQ  = 15;       /* floor(√NUM) */

    if (n == 0u) return (seq_t)SQ;   /* first term */

    seq_t p_im2 = (seq_t)0;  /* p_{-2} */
    seq_t p_im1 = (seq_t)1;  /* p_{-1} */
    seq_t p_i;               /* current numerator */

    /* continued‑fraction parameters */
    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a_k = SQ;          /* a_0 */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* p_k = a_k * p_{k-1} + p_{k-2} */
        p_i = ((seq_t)a_k) * p_im1 + p_im2;
        if (i == n) return p_i;

        /* advance to next partial quotient */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (NUM - m_next * m_next) / d;
        idx_t a_next = (SQ + m_next) / d_next;

        /* rotate previous values */
        m   = m_next;
        d   = d_next;
        a_k = a_next;
        p_im2 = p_im1;
        p_im1 = p_i;
    }

    /* unreachable, but keeps compiler happy */
    return (seq_t)0;
}
