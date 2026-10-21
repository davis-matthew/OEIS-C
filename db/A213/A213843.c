/*
 * A213843  Antidiagonal sums of the convolution array A213841.
 * https://oeis.org/A213843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(1 - 2*n + 4*n^2)/6
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 238, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t a1 = nn * (nn + 1) / 2;              // n*(n+1)/2
    seq_t nn_sq = nn * nn;
    seq_t b = 1 + 4 * nn_sq - 2 * nn;         // 1 + 4*n^2 - 2*n
    return (a1 * b) / 3;                      // divide by remaining 3
}
