/*
 * A173036  a(n) = binomial(n+1, 2) + 13.
 * https://oeis.org/A173036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n*(n+1)/2 + 13
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t tri = tn * (tn + 1) / 2;
    tri += (seq_t)13;
    return tri;
}
