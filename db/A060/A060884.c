/*
 * A060884  a(n) = n^4 - n^3 + n^2 - n + 1.
 * https://oeis.org/A060884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - n^3 + n^2 - n + 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;        /* n^2 */
    seq_t x3 = x2 * x;       /* n^3 */
    seq_t x4 = x3 * x;       /* n^4 */
    seq_t res = x4 - x3 + x2 - x + (seq_t)1;
    return res;
}
