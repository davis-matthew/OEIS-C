/*
 * A001526  a(n) = (7*n+1)*(7*n+6).
 * https://oeis.org/A001526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n+1)*(7*n+6)
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return (7*nn + 1) * (7*nn + 6);
}
