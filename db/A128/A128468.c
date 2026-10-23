/*
 * A128468  a(n) = 30*n + 17.
 * https://oeis.org/A128468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*n + 17
 * Range:     u8 n <= 7, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)30 * n + (seq_t)17;
}
