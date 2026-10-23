/*
 * A175831  Partial sums of ceiling(n^2/12).
 * https://oeis.org/A175831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(2*n^2 + n + 41) / 72)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1947, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the partial sums of ceil(n^2/12) for OEIS A175831. */
seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t1 = nn + 1;
    seq_t t2 = 2*nn*nn + nn + 41;
    seq_t prod = t1 * t2;
    return prod / 72;
}
