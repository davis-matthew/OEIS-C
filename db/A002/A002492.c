/*
 * A002492  Sum of the first n even squares: a(n) = 2*n*(n+1)*(2*n+1)/3.
 * https://oeis.org/A002492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n*(n+1)*(2*n+1)/3
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 2 * nn * (nn + 1) * (2 * nn + 1) / 3;
}
