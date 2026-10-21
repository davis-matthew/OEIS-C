/*
 * A082759  a(n) = Sum_{k = 0..n} binomial(n,k)*trinomial(n,k), where
 *          trinomial(n,k) = trinomial coefficients.
 * https://oeis.org/A082759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k) * binom(2*n - k, k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t binom_nk = 1;         /* C(n,0) */
    seq_t binom_2nk_k = 1;      /* C(2*n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* accumulate term */
        result += binom_nk * binom_2nk_k;

        if (k == n) break; /* last iteration done */

        /* recurrence: C(n,k+1) = C(n,k) * (n - k) / (k + 1) */
        binom_nk = binom_nk * ( (seq_t)(n - k) ) / ( (seq_t)(k + 1) );

        /* recurrence for C(2*n - k, k):
           C(2n-k-1, k+1) = C(2n-k, k) *
             (2n - 2k) * (2n - 2k - 1) / ((k+1) * (2n - k)) */
        seq_t num1 = (seq_t)(2 * n - 2 * k);         /* 2n - 2k */
        seq_t num2 = (seq_t)(2 * n - 2 * k - 1);     /* 2n - 2k - 1 */
        seq_t den1 = (seq_t)(k + 1);                 /* k + 1 */
        seq_t den2 = (seq_t)(2 * n - k);             /* 2n - k */

        binom_2nk_k = binom_2nk_k * num1;
        binom_2nk_k = binom_2nk_k * num2;
        binom_2nk_k = binom_2nk_k / den1;
        binom_2nk_k = binom_2nk_k / den2;
    }

    return result;
}
