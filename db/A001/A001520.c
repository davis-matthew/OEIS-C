/*
 * A001520  a(n) = (6*n+1)*(6*n+3)*(6*n+5).
 * https://oeis.org/A001520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n+1)*(6*n+3)*(6*n+5)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 270, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return (6 * x + 1) * (6 * x + 3) * (6 * x + 5);
}
