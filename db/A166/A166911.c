/*
 * A166911  a(n) = (9 + 14*n + 12*n^2 + 4*n^3)/3.
 * https://oeis.org/A166911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9 + 14*n + 12*n^2 + 4*n^3)/3
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 1153, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t term = (seq_t)9 + (seq_t)14 * nn + (seq_t)12 * n2 + (seq_t)4 * n3;
    return term / (seq_t)3;
}
