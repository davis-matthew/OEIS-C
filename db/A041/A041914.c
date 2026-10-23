/*
 * A041914  Numerators of continued fraction convergents to sqrt(479).
 * https://oeis.org/A041914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(479)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: h_{-1} = 1, h_0 = floor(sqrt(479)) = 21
 * h_n     = a_n * h_{n-1} + h_{n-2},  where a_n are the partial
 * quotients of the continued fraction expansion of sqrt(479).  */
/* The partial quotients are generated using the standard recurrence
 * for quadratic surds.  */

seq_t a(idx_t n)
{
    /* Base integer part of sqrt(479).  */
    const seq_t a0 = 21;

    /* Base case: the 0-th numerator is the integer part itself.  */
    if (n == 0)
        return a0;

    /* Numerators of the convergents.  */
    seq_t h_minus1 = 1;   /* Corresponds to h_{-1} in the recurrence. */
    seq_t h = a0;        /* h_0  – the 0-th numerator.              */

    /* State variables for generating continued fraction coefficients.  */
    seq_t m = 0;
    seq_t d = 1;
    seq_t coeff = a0;    /* Current coefficient a_k (starts with a0). */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next m, d and the next coefficient a_{k+1}.  */
        seq_t m_new = d * coeff - m;
        seq_t d_new = ((seq_t)479 - m_new * m_new) / d;
        seq_t coeff_new = (a0 + m_new) / d_new;

        /* Update numerator using the recurrence. */
        seq_t h_new = coeff_new * h + h_minus1;
        h_minus1 = h;
        h = h_new;

        /* Prepare for next iteration.  */
        m = m_new;
        d = d_new;
        coeff = coeff_new;
    }

    return h;
}
