/*
 * A042034  Numerators of continued fraction convergents to sqrt(541).
 * https://oeis.org/A042034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042034: Numerators of the convergents to √541.
   The numbers are generated from the continued‑fraction of √541:
     a₀ = ⌊√541⌋ = 23
   For k ≥ 0 the partial quotients a_k are given by the classical
   Pell‑algorithm:
     m₀ = 0, d₀ = 1, a₀ = 23
     m_{k+1} = d_k·a_k − m_k
     d_{k+1} = (541 − m_{k+1}²) / d_k
     a_{k+1} = ⌊(a₀ + m_{k+1}) / d_{k+1}⌋
   The convergent numerators p_k satisfy the recurrence
     p_{−2} = 0, p_{−1} = 1
     p_k   = a_k · p_{k−1} + p_{k−2}
   a(0) = p₀ = 23, a(1) = p₁ = 70, …  */
seq_t a(idx_t n)
{
    /* constants for √541 */
    const long long D   = 541;          /* D = 541 */
    const long long a0  = 23;           /* a₀ = floor(√ξ) */

    /* state of the continued fraction generation */
    long long m   = 0;
    long long d   = 1;
    long long a_k = a0;                 /* current partial quotient */

    /* recurrence for the numerators */
    seq_t p_prevprev = 0;   /* p_{k-2}  */
    seq_t p_prev     = 1;   /* p_{k-1}  */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute current numerator p_k */
        seq_t p = (seq_t)a_k * p_prev + p_prevprev;
        if (k == n) return p;

        /* advance the continued‑fraction state to a_{k+1} */
        long long m_next = d * a_k - m;
        long long d_next = (D - m_next * m_next) / d;
        long long a_next = (a0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_k = a_next;

        /* shift the numerator recurrence */
        p_prevprev = p_prev;
        p_prev     = p;
    }
    /* unreachable but keeps compiler satisfied */
    return 0;
}
