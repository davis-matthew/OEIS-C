/*
 * A107418  a(n) = binomial(n+3,3)*binomial(n+6,6).
 * https://oeis.org/A107418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+3,3) * binomial(n+6,6)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;            /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i);   /* multiply by next factor */
        res = res / (seq_t)i;             /* divide by the denominator */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t b3 = binom_small(n + 3, 3);
    seq_t b6 = binom_small(n + 6, 6);
    return b3 * b6;
}
