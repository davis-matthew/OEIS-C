/*
 * A041548  Numerators of continued fraction convergents to sqrt(292).
 * https://oeis.org/A041548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of nth convergent of the continued fraction for
 *            sqrt(292). Let a_0 = floor(sqrt(292)) = 17. For n ≥ 1, let a_n
 *            be the nth partial quotient of sqrt(292) obtained by the
 *            standard quadratic‑surd algorithm. The numerators p_n satisfy
 *            p_{-2}=0, p_{-1}=1, and for n ≥ 0: p_n = a_n * p_{...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Parameters for sqrt(292) */
    const uint64_t D     = 292ULL;
    const uint64_t a0    = 17ULL;     /* floor(sqrt(292)) */

    /* Variables for the continued‑fraction algorithm */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* Two previous convergents (p and q) */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Current partial quotient is 'a' */
        seq_t term_a = (seq_t)a;
        seq_t p = term_a * p_prev1 + p_prev2;
        seq_t q = term_a * q_prev1 + q_prev2;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;

        /* Compute next partial quotient if not finished */
        if (k < n) {
            uint64_t m_next = d * a - m;
            m = m_next;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
        }
    }

    /* After the loop, p_prev1 holds p_n */
    return p_prev1;
}
