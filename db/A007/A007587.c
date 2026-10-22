/*
 * A007587  12-gonal (or dodecagonal) pyramidal numbers: a(n) =
 *          n*(n+1)*(10*n-7)/6.
 * https://oeis.org/A007587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(10*n-7)/6
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 992, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t p1 = m * (m + 1) / 2;
    seq_t p2 = p1 * (10 * m - 7);
    return p2 / 3;
}
