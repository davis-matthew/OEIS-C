/*
 * A081435  Diagonal in array of n-gonal numbers A081422.
 * https://oeis.org/A081435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 +3*n^2 +3*n +2)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;          /* n^2 */
    seq_t n3 = n2 * tn;          /* n^3 */
    seq_t val = 2 * n3 + 3 * n2 + 3 * tn + 2; /* (2n^3 +3n^2 +3n +2) */
    return val / 2;
}
