/*
 * A123868  a(n) = n^12 - 1.
 * https://oeis.org/A123868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^12 - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to the sequence type */
    seq_t x = n;
    /* Compute powers by repeated squaring: n^2, n^4, n^8, then n^12 = n^8 * n^4 */
    seq_t x2 = x * x;
    seq_t x4 = x2 * x2;
    seq_t x8 = x4 * x4;
    seq_t x12 = x8 * x4;
    /* a(n) = n^12 - 1 */
    return x12 - 1;
}
