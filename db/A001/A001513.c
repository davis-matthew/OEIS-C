/*
 * A001513  a(n) = (6*n+1)*(6*n+5).
 * https://oeis.org/A001513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n+1)*(6*n+5)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n;
    return (6*t + 1) * (6*t + 5);
}
