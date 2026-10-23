/*
 * A110451  a(n) = n*(4*n^2 + 2*n + 1).
 * https://oeis.org/A110451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^2 + 2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1023, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t part = (seq_t)4 * N * N + (seq_t)2 * N + (seq_t)1;
    return N * part;
}
