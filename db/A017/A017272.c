/*
 * A017272  a(n) = (10*n)^4.
 * https://oeis.org/A017272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10000 * n^4 (i.e. (10*n)^4)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 25, u64 n <= 250, u128 n <= 250,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x  = (seq_t)n;          /* convert index to seq_t */
    seq_t x2 = x * x;            /* n^2 */
    seq_t x4 = x2 * x2;          /* n^4 */
    return x4 * 10000;           /* multiply by 10^4 */
}
