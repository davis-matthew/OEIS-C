/*
 * A042572  Numerators of continued fraction convergents to sqrt(815).
 * https://oeis.org/A042572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=28; for n≥1, a(n)=A[n]*a(n‑1)+a(n‑2), * where A[n] is the
 *            n‑th partial quotient of the continued fraction of √815. * The
 *            partial quotients (excluding the initial 28) repeat with period
 *            12: * A[1..12] = {1,1,4,1,2,5,2,1,4,1,1,56}
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th numerator of the convergents to sqrt(815). */
seq_t a(idx_t n)
{
    /* Periodic partial quotients after the 0th term. */
    const unsigned int cf[12] = {1, 1, 4, 1, 2, 5, 2, 1, 4, 1, 1, 56};

    /* Base case: the 0th convergent has numerator 28. */
    if (n == 0)
        return (seq_t)28;

    /* Recurrence: p_{-1}=1, p_0=28, p_k = a_k * p_{k-1} + p_{k-2}. */
    seq_t p_prev2 = (seq_t)1;   /* p_{-1} */
    seq_t p_prev  = (seq_t)28;  /* p_0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t a_i = (seq_t)cf[(i - 1) % 12];  /* partial quotient a_i */
        seq_t p_new = a_i * p_prev + p_prev2; /* p_i */
        p_prev2 = p_prev;
        p_prev  = p_new;
    }

    return p_prev; /* p_n */
}
