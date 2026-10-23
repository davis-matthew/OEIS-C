/*
 * A140064  a(n) = (9*n^2 - 5*n + 2)/2.
 * https://oeis.org/A140064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 - 5*n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t = (seq_t)9 * nn * nn + (seq_t)2;
    t -= (seq_t)5 * nn;
    return t / 2;
}
