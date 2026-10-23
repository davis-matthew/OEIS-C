/*
 * A161617  a(n) = 8*n^2 + 20*n + 1.
 * https://oeis.org/A161617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n^2 + 20*n + 1
 * Range:     u8 n <= 4, u16 n <= 89, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t first to preserve precision */
    seq_t v = (seq_t)n;
    /* Compute 8*v*v + 20*v + 1 */
    return v * v * 8 + v * 20 + 1;
}
