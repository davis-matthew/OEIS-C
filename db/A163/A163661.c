/*
 * A163661  a(n) = n*(2*n^2 + 5*n + 17)/2.
 * https://oeis.org/A163661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 5*n + 17)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t n_sq = N * N;
    seq_t expr = 2 * n_sq + 5 * N + 17;
    if (N & 1) {
        expr = expr / 2;
        return N * expr;
    } else {
        return (N / 2) * expr;
    }
}
