/*
 * A158553  a(n) = 196*n^2 - 14.
 * https://oeis.org/A158553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 196*n^2 - 14
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset is 1; for safety */
    seq_t nn = (seq_t)n;
    return nn * nn * (seq_t)196 - (seq_t)14;
}
