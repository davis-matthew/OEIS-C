/*
 * A226450  a(n) = n*(3*n^2 - 5*n + 3).
 * https://oeis.org/A226450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n^2 - 5*n + 3)
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    seq_t n2  = n_s * n_s;
    seq_t inner = 3 * n2 - 5 * n_s + 3;
    return n_s * inner;
}
