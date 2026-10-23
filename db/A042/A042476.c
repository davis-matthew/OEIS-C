/*
 * A042476  Numerators of continued fraction convergents to sqrt(766).
 * https://oeis.org/A042476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let a₀ = 27 and let {a_k} be the partial quotients of the continued fraction for sqrt(766).  
   The convergent numerators satisfy  
     p_{-2} = 0,  p_{-1} = 1,  
     p_k   = a_k · p_{k-1} + p_{k-2}   (k ≥ 0).  
   The function returns p_n. */

seq_t a(idx_t n)
{
    /* Constants in seq_t form */
    const seq_t A0 = (seq_t)27;   /* integer part of sqrt(766) */
    const seq_t D  = (seq_t)766;  /* the radicand */

    if (n == 0)
        return A0; /* p_0 = a₀*1 + 0 = 27 */

    /* Initialize for the continued fraction algorithm */
    seq_t m   = (seq_t)0;
    seq_t d   = (seq_t)1;
    seq_t a_k = A0;               /* a₀ */

    /* Convergent numerators */
    seq_t p_nm2 = (seq_t)0;  /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;  /* p_{-1} */
    seq_t p_n;               /* current p_k */

    idx_t k;  /* index of current convergent */
    for (k = 0; ; ++k) {
        /* Compute p_k */
        p_n = a_k * p_nm1 + p_nm2;

        /* If we've reached the requested index, return the result */
        if (k == n)
            return p_n;

        /* Prepare next partial quotient a_{k+1} using the standard algorithm */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        /* Shift for the next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }
}
