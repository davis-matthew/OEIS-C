/*
 * A083080  Main diagonal of number array A083075.
 * https://oeis.org/A083080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*(2n+3)^n + 1)/(n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 0 using a numerically stable summation that */
/* avoids overflow when seq_t is uint64_t (works for all n that fit in 64 bits). */
seq_t a(idx_t n)
{
    /* m = n + 1  (used for both arithmetic and loop limits) */
    seq_t m  = (seq_t)n + 1;
    idx_t  m_idx = (idx_t)m;          /* safe conversion – n fits in idx_t */

    /* Summation of T = Σ_{k=1}^{m-1} C(m-1,k) * 2^k * m^{k-1} */
    seq_t T     = 0;
    seq_t binom = 1; /* C(m-1,0) */
    seq_t pow2  = 1; /* 2^0 */
    seq_t powM  = 1; /* m^0 */

    for (idx_t k = 1; k < m_idx; ++k) {
        /* Update binomial coefficient: C(m-1,k) = C(m-1,k-1) * (m-1-(k-1)) / k */
        binom = binom * (((seq_t)(m - 1) - (seq_t)(k - 1))) / (seq_t)k;

        pow2  = pow2 * 2;
        /* term = C(m-1,k) * 2^k * m^{k-1} */
        seq_t term = binom * pow2 * powM;
        T = T + term;

        powM = powM * m;   /* prepare m^(k) for next iteration */
    }

    /* a(n) = 1 + (m-1) * T   (where m-1 = n) */
    return 1 + (m - 1) * T;
}
