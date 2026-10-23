/*
 * A171965  Partial sums of floor(n^2/6) (A056827).
 * https://oeis.org/A171965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 12*n + 7)/36)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 2205, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* cast index to sequence type */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t num = 2 * n3 + 3 * n2 - 12 * nn + 7; /* numerator */
    return num / 36;              /* integer division gives floor */
}
