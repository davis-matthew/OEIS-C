/*
 * A100193  a(n) = Sum_{k=0..n} binomial(2*n,n+k)*3^k.
 * https://oeis.org/A100193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n, n+k) * 3^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient C(2n, n) */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * ((seq_t)n + i) / (seq_t)i;
    }

    seq_t sum = binom;        /* term for k = 0 (3^0 = 1) */
    seq_t pow3 = (seq_t)1;    /* current value of 3^k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial: C(2n, n+k) = C(2n, n+k-1) * (n - k + 1) / (n + k) */
        seq_t num = (seq_t)(n - k + 1);
        seq_t den = (seq_t)(n + k);
        binom = binom * num / den;

        pow3 = pow3 * (seq_t)3;    /* 3^k */
        sum = sum + binom * pow3;
    }

    return sum;
}
