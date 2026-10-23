/*
 * A135389  Number of walks of length 2*n+2 from origin to (1,1) in a square
 *          lattice.
 * https://oeis.org/A135389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(n) = binomial(2*n+2, n) * binomial(2*n+2, n+1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_simple(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;            /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = (res * (seq_t)(N - K + i)) / (seq_t)i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t m = 2 * n + 2;
    seq_t b1 = binom_simple(m, n);
    seq_t b2 = binom_simple(m, n + 1);
    return b1 * b2;
}
