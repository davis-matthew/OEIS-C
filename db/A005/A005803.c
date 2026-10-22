/*
 * A005803  Second-order Eulerian numbers: a(n) = 2^n - 2*n.
 * https://oeis.org/A005803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^n - 2*n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n; i++) {
        power *= (seq_t)2;
    }
    return power - (seq_t)2 * (seq_t)n;
}
