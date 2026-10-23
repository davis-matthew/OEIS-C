/*
 * A042639  Denominators of continued fraction convergents to sqrt(849).
 * https://oeis.org/A042639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the (n‑th) convergent to sqrt(849)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated from the continued fraction expansion of
   sqrt(849).  Starting with a0 = floor(sqrt(849)) = 29 and using the
   standard recurrence

          h_n = a_n * h_{n-1} + h_{n-2}
          k_n = a_n * k_{n-1} + k_{n-2}

   where k_n is the denominator of the n‑th convergent.
   The partial quotients a_n are obtained from
          m_{i+1} = d_i * a_i - m_i
          d_{i+1} = (849 - m_{i+1}^2) / d_i,
          a_{i+1} = floor((29 + m_{i+1}) / d_{i+1}).
   The algorithm runs in O(n) time and uses only integer arithmetic. */

seq_t a(idx_t n)
{
    if (n == 0)               /* the first denominator is 1 */
        return (seq_t)1;

    /* Initial data for the continued fraction of sqrt(849) */
    const seq_t a0 = (seq_t)29;          /* floor(sqrt(849)) */
    idx_t      m   = 0;                   /* m_0 */
    idx_t      d   = 1;                   /* d_0 */

    /* Convergent recurrence variables: h_{-2}, h_{-1}, k_{-2}, k_{-1} */
    seq_t h_prev = (seq_t)1;   /* h_{-1} */
    seq_t k_prev = (seq_t)0;   /* k_{-1} */

    /* First convergent (n = 0) */
    seq_t h_cur  = a0 * h_prev;        /* h_0 = a0*h_{-1} + h_{-2} = a0 */
    seq_t k_cur  = a0 * k_prev + (seq_t)1; /* k_0 = a0*k_{-1} + k_{-2} = 1 */

    /* Variables for continued‑fraction generation */
    seq_t prev_a = a0;                /* a_i, starting with a0 */
    idx_t  m_cur = m;
    idx_t  d_cur = d;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next partial quotient a_next and the corresponding m, d */
        seq_t m_next = (seq_t)d_cur * prev_a - m_cur;
        seq_t d_next = (seq_t)((seq_t)849 - m_next * m_next) / d_cur;
        seq_t a_next = (seq_t)(a0 + m_next) / d_next;

        /* Update the convergent denominators */
        seq_t h_next = a_next * h_cur + h_prev;
        seq_t k_next = a_next * k_cur + k_prev;

        /* Shift variables for next iteration */
        h_prev = h_cur;  h_cur = h_next;
        k_prev = k_cur;  k_cur = k_next;

        prev_a = a_next;
        m_cur  = (idx_t)m_next;
        d_cur  = (idx_t)d_next;
    }

    return k_cur;        /* k_n */
}
