/*
 * A217280  a(n) = Sum_{k=0..n} binomial(n,k)^3 * 2^k * 3^(n-k).
 * https://oeis.org/A217280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * 2^k * 3^(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  result = Σ C(n,k)^3 * 2^k * 3^(n-k)  (k = 0..n) */
    seq_t result = (seq_t)0;
    seq_t binom  = (seq_t)1;   /* C(n,0) */
    seq_t pow2   = (seq_t)1;   /* 2^0   */
    seq_t pow3   = (seq_t)1;   /* 3^n   */

    /* Compute 3^n once */
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    /* Loop over k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k) {
        /* term = (C(n,k)^3) * 2^k * 3^(n-k) */
        seq_t term = binom * binom;  /* binom^2 */
        term *= binom;               /* binom^3 */
        term *= pow2;
        term *= pow3;

        result += term;

        /* Prepare values for next k */
        if (k == n) break;

        /* Update binomial: C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        binom *= (seq_t)(n - k);
        binom /= (seq_t)(k + 1);

        /* Update powers: 2^(k+1) = 2^k * 2, 3^(n-(k+1)) = 3^(n-k) / 3 */
        pow2 *= (seq_t)2;
        pow3 /= (seq_t)3;
    }

    return result;
}
