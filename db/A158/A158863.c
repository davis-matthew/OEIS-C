/*
 * A158863  Maximal excess of a 3-normalized Hadamard matrix of order 4n.
 * https://oeis.org/A158863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = 2*n*n for even n a(n) = n*n + 12*n - 9 for odd n
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n * (seq_t)n;
    if ((n & 1) == 0) {
        return nn + nn;                     /* 2*n^2 */
    } else {
        return nn + (seq_t)12 * (seq_t)n - (seq_t)9;  /* n^2 + 12*n - 9 */
    }
}
