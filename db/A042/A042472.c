/*
 * A042472  Numerators of continued fraction convergents to sqrt(764).
 * https://oeis.org/A042472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 18, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p_{-2}=0, p_{-1}=1. For each n>=0 let
   a_n be the nth partial quotient of the continued fraction of
   sqrt(764) (computed by the standard recurrence).  Then
   p_n = a_n * p_{n-1} + p_{n-2}.  a(0) returns p_0, etc. */
seq_t a(idx_t n)
{
    const seq_t A0 = (seq_t)27;         /* floor(sqrt(764))            */
    const idx_t D  = 764;               /* the non‑square under the root */

    /* n == 0 is handled naturally by the loop. */
    seq_t p_prev2 = (seq_t)0;           /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;           /* p_{-1} */
    seq_t m       = (seq_t)0;           /* m_0   */
    seq_t d       = (seq_t)1;           /* d_0   */
    seq_t a_coeff = A0;                 /* a_0   */

    for (idx_t k = 0; k <= n; ++k) {
        /* p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p_k = a_coeff * p_prev1 + p_prev2;

        /* shift state for next iteration */
        seq_t tmp_prev2 = p_prev1;
        seq_t tmp_prev1 = p_k;
        p_prev2 = tmp_prev2;
        p_prev1 = tmp_prev1;

        if (k == n) return p_k;       /* finished, return p_n */

        /* compute next partial quotient a_{k+1} via continued fraction */
        seq_t m_next = d * a_coeff - m;
        seq_t d_next = (seq_t)D - m_next * m_next;
        d_next = d_next / d;                      /* exact division */
        seq_t a_next = (A0 + m_next) / d_next;    /* floor((A0+m_next)/d_next) */

        /* advance state */
        m = m_next;
        d = d_next;
        a_coeff = a_next;
    }
    /* unreachable, but required for compilability */
    return 0;
}
