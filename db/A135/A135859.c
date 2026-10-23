/*
 * A135859  Row sums of triangle A135858.
 * https://oeis.org/A135859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - 3*n^2 + 5*n - 2 (OEIS A135859)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t t2 = 3 * n2;           /* 3*n^2 */
    seq_t t3 = 5 * nn;           /* 5*n  */
    return n3 - t2 + t3 - 2;     /* n^3 - 3*n^2 + 5*n - 2 */
}
