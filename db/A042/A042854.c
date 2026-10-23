/*
 * A042854  Numerators of continued fraction convergents to sqrt(958).
 * https://oeis.org/A042854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A042854 – numerators of continued‑fraction convergents to √958.
   Recurrence:
     let D   = 958
     a0     = ⌊√D⌋
     m_0    = 0,   d_0    = 1,   a_0 = ⌊√D⌋
     for k ≥ 0
       p_{-2} = 0,   p_{-1} = 1
       p_k   = a_k·p_{k‑1} + p_{k‑2}          (numerators)
       m_{k+1} = d_k·a_k – m_k
       d_{k+1} = (D – m_{k+1}²) / d_k
       a_{k+1} = ⌊ (a0 + m_{k+1}) / d_{k+1} ⌋
*/

seq_t a(idx_t n)
{
    const idx_t D = 958;

    /* integer floor‑sqrt of D (D is fixed small so no overflow) */
    idx_t a0;
    {
        idx_t l = 0, h = D;
        while (l < h) {
            idx_t m = (l + h + 1) >> 1;
            if (m * m <= D) l = m;
            else           h = m - 1;
        }
        a0 = l;
    }

    idx_t m   = 0;          /* m_k   */
    idx_t d   = 1;          /* d_k   */
    idx_t a_k = a0;         /* a_k   */

    seq_t p_m2 = 0;         /* p_{k‑2} */
    seq_t p_m1 = 1;         /* p_{k‑1} */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t p_k = ((seq_t)a_k) * p_m1 + p_m2;
        if (k == n) return p_k;

        /* step to next partial quotient */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_k = a_next;

        p_m2 = p_m1;
        p_m1 = p_k;
    }
    return 0; /* never reached */
}
