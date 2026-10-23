/*
 * A195041  Concentric heptagonal numbers.
 * https://oeis.org/A195041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + floor(3*n^2/4)
 * Range:     u8 n <= 12, u16 n <= 193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t part = (seq_t)3 * n2 / 4;
    return n2 + part;
}
