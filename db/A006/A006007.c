/*
 * A006007  4-dimensional analog of centered polygonal numbers: a(n) =
 *          n(n+1)*(n^2+n+4)/12.
 * https://oeis.org/A006007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n*(n+1)*(n^2+n+4)/12
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 290, u64 n <= 710,
 *            u128 n <= 710, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    seq_t m = n_s * (n_s + 1);
    seq_t numerator = m * (m + 4);
    return numerator / 12;
}
