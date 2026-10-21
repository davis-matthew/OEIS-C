/*
 * A209265  a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 +5*n^5 + 6*n^6 + 7*n^7.
 * https://oeis.org/A209265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5 + 6*n^6 + 7*n^7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 427, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t n7 = n6 * nn;
    return (seq_t)1
           + (seq_t)2 * n2
           + (seq_t)3 * n3
           + (seq_t)4 * n4
           + (seq_t)5 * n5
           + (seq_t)6 * n6
           + (seq_t)7 * n7;
}
