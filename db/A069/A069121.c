/*
 * A069121  a(n) = n^4*binomial(2*n,n).
 * https://oeis.org/A069121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4 * binomial(2*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t binom = 1;
    for (idx_t k = 1; k <= n; ++k) {
        binom = binom * (n + k);
        binom = binom / k;
    }

    seq_t res = binom;
    seq_t nseq = (seq_t)n;
    res *= nseq;
    res *= nseq;
    res *= nseq;
    res *= nseq;

    return res;
}
