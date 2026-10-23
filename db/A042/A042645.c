/*
 * A042645  Denominators of continued fraction convergents to sqrt(852).
 * https://oeis.org/A042645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(852).
   The recurrence used is the standard one for a continued fraction:
   k_{-1}=0, k_0=1, k_n = a_n k_{n-1} + k_{n-2}.
   For sqrt(D) the partial quotients a_n are produced by
       m_{n+1} = d_n a_n - m_n
       d_{n+1} = (D - m_{n+1}²) / d_n
       a_{n+1} = ⌊ (a_0 + m_{n+1}) / d_{n+1} ⌋ ,
   where a_0 = ⌊√D⌋.  */
seq_t a(idx_t n)
{
    /* target square‑free integer */
    const idx_t D  = 852u;          /* sqrt(D) = 29.xxx */
    const idx_t a0 = 29u;           /* floor(sqrt(D)) */

    if (n == 0u) {
        return (seq_t)1;
    }

    idx_t m      = 0u;
    idx_t d      = 1u;
    idx_t a_val  = a0;              /* a_n at the current step */

    seq_t k_prev = 0u;              /* k_{n-1} */
    seq_t k_cur  = 1u;              /* k_n   */

    for (idx_t i = 0u; i < n; ++i) {
        idx_t m_next = d * a_val - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;   /* floor division */

        seq_t k_new = (seq_t)a_next * k_cur + k_prev;

        k_prev = k_cur;
        k_cur  = k_new;

        m      = m_next;
        d      = d_next;
        a_val  = a_next;
    }

    return k_cur;
}
