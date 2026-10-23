/*
 * A041890  Numerators of continued fraction convergents to sqrt(467).
 * https://oeis.org/A041890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) is the numerator of the n‑th continued fraction convergent
 *            to √467. The numerators satisfy the recurrence p_{-2} = 0,
 *            p_{-1} = 1, p_k = a_k * p_{k-1} + p_{k-2}, where the a_k are the
 *            simple continued fraction coefficients of √467 computed by the
 *            standard period algorithm: m_0 = 0, d_0 = 1, a...
 * Range:     u8 none, u16 n <= 7, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* external types */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* constant for the quadratic irrational √467 */
    const idx_t S = (idx_t)467;

    /* initial data for the continued fraction of √S */
    idx_t a0 = (idx_t)floor_sqrt((seq_t)S);   /* ⌊√467⌋ = 21 */

    /* numerators */
    seq_t p_m2 = (seq_t)0;      /* p_{-2} */
    seq_t p_m1 = (seq_t)1;      /* p_{-1} */
    seq_t p_curr = (seq_t)0;    /* will hold p_k */

    /* special case n = 0 */
    if (n == 0)
        return (seq_t)a0 * p_m1 + p_m2;  /* ≡ p_0 = a0 */

    /* first convergent (k = 0) */
    p_curr = (seq_t)a0 * p_m1 + p_m2;    /* p_0 */
    p_m2 = p_m1;
    p_m1 = p_curr;

    /* continued fraction parameters */
    idx_t m = (idx_t)0;
    idx_t d = (idx_t)1;
    idx_t a_k = a0;          /* current coefficient a_k */

    /* compute subsequent convergents up to n */
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* next continued fraction coefficient */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (S - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_k = a_next;

        /* compute p_k */
        seq_t a_seq = (seq_t)a_k;          /* promote to seq_t for multiplication */
        seq_t p_next = a_seq * p_m1 + p_m2;

        p_m2 = p_m1;
        p_m1 = p_next;
        p_curr = p_next;
    }

    return p_curr;
}
