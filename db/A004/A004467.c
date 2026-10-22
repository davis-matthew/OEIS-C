/*
 * A004467  a(n) = n*(11*n^2 - 5)/6.
 * https://oeis.org/A004467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(11*n^2 - 5)/6
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1327, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    return n_val * (11 * n_val * n_val - 5) / 6;
}
