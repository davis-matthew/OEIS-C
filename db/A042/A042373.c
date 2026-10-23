/*
 * A042373  Denominators of continued fraction convergents to sqrt(713).
 * https://oeis.org/A042373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 10572734*a(n-14) - a(n-28) (denominators of
 *            continued‑fraction convergents to √713) Initial terms: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=7, a(4)=10, a(5)=47, a(6)=292, a(7)=631,
 *            a(8)=4078, a(9)=16943, a(10)=21021, a(11)=58985, a(12)=138991,
 *            a(13)=197976
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* D = 713 */
    const idx_t D = 713;

    /* compute floor(sqrt(D)) */
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) {
        ++a0;
    }

    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* continued–fraction expansion state */
    idx_t m = 0;              /* m_{k-1}  */
    idx_t d = 1;              /* d_{k-1}  */
    idx_t last_a = a0;        /* a_{k-1}  */

    /* convergent denominators */
    seq_t q_prevprev = (seq_t)0;   /* q_{k-2}  */
    seq_t q_prev     = (seq_t)1;   /* q_{k-1}  */
    seq_t q_k        = (seq_t)0;   /* q_{k}    */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* next partial quotient a_i */
        idx_t m_next = d * last_a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* q_i = a_i * q_{i-1} + q_{i-2} */
        q_k = (seq_t)a_next * q_prev + q_prevprev;

        /* shift for next step */
        q_prevprev = q_prev;
        q_prev = q_k;

        /* update continued‑fraction state */
        m = m_next;
        d = d_next;
        last_a = a_next;
    }

    return q_prev;
}
