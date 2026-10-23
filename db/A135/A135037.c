/*
 * A135037  Sums of the products of n consecutive triples of numbers.
 * https://oeis.org/A135037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27*n^4 - 18*n^3 - 15*n^2 + 6*n)/4
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t result = ( (seq_t)27 * n4
                     - (seq_t)18 * n3
                     - (seq_t)15 * n2
                     + (seq_t)6  * nn ) / (seq_t)4;
    return result;
}
