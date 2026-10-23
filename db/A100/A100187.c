/*
 * A100187  Structured octagonal anti-diamond numbers (vertex structure 7).
 * https://oeis.org/A100187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (26*n^3 - 30*n^2 + 10*n) / 6
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 785, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t term = (seq_t)26 * n3 - (seq_t)30 * n2 + (seq_t)10 * nn;
    return term / 6;
}
