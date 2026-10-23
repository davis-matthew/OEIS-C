/*
 * A041283  Denominators of continued fraction convergents to sqrt(154).
 * https://oeis.org/A041283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 42590*a(n-10) - a(n-20) for n ≥ 20; alternatively, a(n)
 *            is the denominator of the n‑th convergent to sqrt(154). The
 *            recurrence can be applied by generating the continued‑fraction
 *            coefficients: a₁ = 2, a₂ = 2, a₃ = 3, a₄ = 1, a₅ = 2, a₆ = 1, a₇
 *            = 3, a₈ = 2, a₉ = 2, a₁₀ = 24 (periodic of len...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term of OEIS A041283. */
seq_t a(idx_t n)
{
    /* Continued‑fraction coefficients for sqrt(154), excluding a₀ = 12. */
    static const seq_t cf[10] = {2, 2, 3, 1, 2, 1, 3, 2, 2, 24};

    if (n == (idx_t)0)                    /* first term */
        return (seq_t)1;

    if (n == (idx_t)1)                    /* second term */
        return (seq_t)2;

    /* q₀ = 1, q₁ = 2 */
    seq_t q_prev2 = (seq_t)1;              /* q_{n-2} */
    seq_t q_prev1 = (seq_t)2;              /* q_{n-1} */

    for (idx_t k = (idx_t)2; k <= n; ++k) {
        /* a_k uses the periodic continued‑fraction coefficients. */
        seq_t a_k = cf[(k - (idx_t)1) % (idx_t)10];
        seq_t q_k = a_k * q_prev1 + q_prev2;   /* recurrence q_n = a_n q_{n-1} + q_{n-2} */

        q_prev2 = q_prev1;
        q_prev1 = q_k;
    }

    return q_prev1;
}
