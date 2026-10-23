/*
 * A132209  a(0) = 0 and a(n) = 2*n^2 + 2*n - 1, for n>=1.
 * https://oeis.org/A132209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 + 2*n - 1 for n>=1, a(0)=0
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    return nn * nn * 2 + nn * 2 - 1;
}
