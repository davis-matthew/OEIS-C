/*
 * A107419  a(n) = binomial(n+4,4)*binomial(n+7,7).
 * https://oeis.org/A107419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4) * binomial(n+7,7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t N, idx_t K)
{
    /* Computes C(N,K) for small K using multiplicative formula.
       Assumes 0 <= K <= N and K fits in idx_t. */
    seq_t r = 1;
    for (idx_t i = 1; i <= K; ++i) {
        r = r * (N - K + i);  /* multiply by (N-K+i) */
        r = r / i;            /* exact division by i */
    }
    return r;
}

seq_t a(idx_t n)
{
    /* a(n) = C(n+4,4) * C(n+7,7) */
    seq_t c1 = binom_small(n + 4, 4);
    seq_t c2 = binom_small(n + 7, 7);
    return c1 * c2;
}
