/*
 * A042486  Numerators of continued fraction convergents to sqrt(771).
 * https://oeis.org/A042486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042486: Numerators of continued fraction convergents to sqrt(771)
   Recurrence: a(n) = 5978273860*a(n-22) - a(n-44) for n > 43
   The following implementation computes a(n) directly by generating the
   continued‑fraction coefficients of sqrt(771) and the convergent numerators.
   It works for any n ≥ 0 and for any seq_t that supports integer arithmetic.
*/

seq_t a(idx_t n)
{
    /* floor(sqrt(771)) = 27 */
    const seq_t a0 = (seq_t)27;

    /* Initial values for recurrence p[-2] = 0, p[-1] = 1 */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;

    /* Continued fraction variables */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_coef = a0;        /* a_0 */

    /* Iterate up to n */
    for (idx_t k = (idx_t)0; k <= n; ++k) {
        /* Numerator of the k-th convergent */
        seq_t p = a_coef * p_prev1 + p_prev2;

        if (k == n) return p;

        /* Compute next continued fraction coefficient */
        seq_t m_next = d * a_coef - m;
        seq_t d_next = ((seq_t)771 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Shift for next iteration */
        m = m_next;
        d = d_next;
        a_coef = a_next;

        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    /* Should never reach here */
    return (seq_t)0;
}
