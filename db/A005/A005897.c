/*
 * A005897  a(n) = 6*n^2 + 2 for n > 0, a(0)=1.
 * https://oeis.org/A005897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 if n = 0; a(n) = 6*n^2 + 2 if n > 0
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t s = (seq_t)n;
    return s * s * 6 + 2;
}
