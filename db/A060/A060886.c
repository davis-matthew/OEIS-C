/*
 * A060886  a(n) = n^4 - n^2 + 1.
 * https://oeis.org/A060886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - n^2 + 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;      /* convert index to seq_t */
    seq_t n2 = nn * nn;       /* n^2   */
    seq_t n4 = n2 * n2;       /* n^4   */
    return n4 - n2 + 1;       /* n^4 - n^2 + 1 */
}
