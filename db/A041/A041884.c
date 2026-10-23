/*
 * A041884  Numerators of continued fraction convergents to sqrt(464).
 * https://oeis.org/A041884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041884: Numerators of continued‑fraction convergents to sqrt(464).
   The recurrence is the standard one for quadratic surds.
   a0    = floor(sqrt(464)) = 21
   For k >= 0:
      m_{k+1} = d_k * a_k - m_k
      d_{k+1} = (464 - m_{k+1}^2) / d_k
      a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})
   Convergent numerators satisfy
      p_{-2} = 0,  p_{-1} = 1
      p_k   = a_k * p_{k-1} + p_{k-2}   (k >= 0)
   a(0) = p_0 = 21, a(1) = p_1 = 22, ...
   The function below implements this recurrence with O(n) time.
   All arithmetic uses the integer types provided by the build. */

seq_t a(idx_t n)
{
    /* Constants for sqrt(464).   */
    const idx_t D   = 464;
    const idx_t a0  = 21;   /* floor(sqrt(464)) */

    /* Special case: first term. */
    if (n == 0)
        return a0;

    /* Continued‑fraction state.   */
    idx_t m   = 0;          /* m_0 */
    idx_t d   = 1;          /* d_0 */
    idx_t a_cf = a0;        /* a_0 */
    idx_t a_next;

    /* Convergent numerators.  p_{-2}=0, p_{-1}=1. */
    seq_t p_prev2 = 0;      /* p_{-2} */
    seq_t p_prev1 = 1;      /* p_{-1} */
    seq_t p_curr;

    /* Compute p_0 explicitly. */
    p_curr = (seq_t)a0 * p_prev1 + p_prev2; /* a0 * 1 + 0  */
    p_prev2 = p_prev1;      /* 0 becomes 1 */
    p_prev1 = p_curr;       /* p_prev1 now holds p_0 */

    /* Iterate to compute p_n.  For i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Next coefficient a_{i}.  Start with current “a_cf” which
           holds a_{i-1}.  Compute m_i, d_i, a_i. */
        idx_t m_next = d * a_cf - m;
        idx_t d_next = (D - m_next * m_next) / d;
        a_next = (a0 + m_next) / d_next;   /* floor division */

        /* Update continued‑fraction state for next iteration. */
        m   = m_next;
        d   = d_next;
        a_cf = a_next;

        /* Compute next convergent numerator p_i. */
        p_curr = (seq_t)a_next * p_prev1 + p_prev2;

        /* Shift for next iteration. */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_prev1;  /* p_n */
}
