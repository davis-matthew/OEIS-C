/*
 * A001547  a(n) = (7*n+1)*(7*n+2)*(7*n+4).
 * https://oeis.org/A001547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n+1)*(7*n+2)*(7*n+4)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 231, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return (7*x + 1) * (7*x + 2) * (7*x + 4);
}
