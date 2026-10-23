/*
 * A042494  Numerators of continued fraction convergents to sqrt(775).
 * https://oeis.org/A042494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the (n‑th) convergent of the continued
 *            fraction for sqrt(775) (OEIS A042494)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t S = (idx_t)775;                     /* D in sqrt(D) */
    /* integer floor sqrt(S) */
    idx_t sqrtS = 0;
    while ((sqrtS + 1) * (sqrtS + 1) <= S) ++sqrtS;

    /* Variables for the continued‑fraction expansion of sqrt(S) */
    unsigned long long m   = 0;         /* m_k */
    unsigned long long d   = 1;         /* d_k */
    unsigned long long a_k = sqrtS;     /* a_k */

    seq_t p_prevprev = (seq_t)0;   /* p_{k-2} */
    seq_t p_prev    = (seq_t)1;    /* p_{k-1} */
    seq_t result    = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = (seq_t)a_k;
        result = ak * p_prev + p_prevprev;   /* p_k */

        if (k == n) return result;           /* correct index reached */

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev = result;

        /* compute next partial quotient a_{k+1} */
        unsigned long long m_next = d * a_k - m;
        unsigned long long d_next = (S - m_next * m_next) / d;
        a_k = (sqrtS + m_next) / d_next;

        /* update m and d for the next step */
        m = m_next;
        d = d_next;
    }
    /* unreachable */
    return result;
}
