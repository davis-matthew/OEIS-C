/*
 * A001296  4-dimensional pyramidal numbers: a(n) = (3*n+1)*binomial(n+2,
 *          3)/4. Also Stirling2(n+2, n).
 * https://oeis.org/A001296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(n+2)*(3*n+1)/24
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 351, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t binom = (n_seq + 2) * (n_seq + 1) / 2;
    binom = binom * n_seq / 3;
    seq_t result = (3 * n_seq + 1) * binom / 4;
    return result;
}
