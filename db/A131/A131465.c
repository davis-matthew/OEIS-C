/*
 * A131465  a(n) = 4*n^4 - 3*n^3 + 2*n^2 - n + 1.
 * https://oeis.org/A131465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^4 - 3*n^3 + 2*n^2 - n + 1
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn     = (seq_t)n;      /* n */
    seq_t n2     = nn * nn;        /* n^2 */
    seq_t n3     = n2 * nn;        /* n^3 */
    seq_t n4     = n3 * nn;        /* n^4 */
    return 4 * n4 - 3 * n3 + 2 * n2 - nn + 1;
}
