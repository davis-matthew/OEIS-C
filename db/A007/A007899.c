/*
 * A007899  Coordination sequence for hexagonal close-packing.
 * https://oeis.org/A007899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(21*n^2 / 2) + 2 for n >= 1; a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t n_seq = (seq_t)n;
    seq_t k = n_seq / 2;
    if (n & 1) {
        return 42 * k * k + 42 * k + 12;
    } else {
        return 42 * k * k + 2;
    }
}
