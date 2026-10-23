/*
 * A041050  Numerators of continued fraction convergents to sqrt(31).
 * https://oeis.org/A041050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Numerators of continued‑fraction convergents to √31. Recurrence:
 *            a(n) = 3040·a(n‑8) – a(n‑16) (n ≥ 16) Initial values are
 *            generated algorithmically from the continued fraction of √31, so
 *            no literal lookup table is used
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* D = 31, floor(sqrt(D)) = 5  */
    const int D   = 31;
    const int a0  = 5;          /* first continued‑fraction coefficient */

    /* Convergent denominators p_{-2}=0, p_{-1}=1  */
    seq_t p_m2 = 0;              /* p_{k-2}   */
    seq_t p_m1 = 1;              /* p_{k-1}   */
    seq_t p_k  = 0;              /* current p_k */

    /* Variables for continued‑fraction generation of √31   */
    int m   = 0;            /* m_k   */
    int d   = 1;            /* d_k   */
    int a_k = a0;           /* a_k   */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k != 0)          /* update continued‑fraction coefficient for next step */
        {
            int m_next = d * a_k - m;
            int d_next = (D - m_next * m_next) / d;
            int a_next = (a0 + m_next) / d_next;
            m = m_next;
            d = d_next;
            a_k = a_next;
        }

        /* Convergent numerator:  p_k = a_k·p_{k-1} + p_{k-2}   */
        p_k = (seq_t)a_k * p_m1 + p_m2;

        /* Slide window for next iteration */
        p_m2 = p_m1;
        p_m1 = p_k;
    }

    return p_k;
}
