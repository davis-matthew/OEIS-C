/*
 * A001621  a(n) = n*(n + 1)*(n^2 + n + 2)/4.
 * https://oeis.org/A001621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n^2 + n + 2)/4
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 360, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t tri = n_val * (n_val + 1) / 2;
    seq_t quad = n_val * n_val + n_val + 2;
    return tri * quad / 2;
}
