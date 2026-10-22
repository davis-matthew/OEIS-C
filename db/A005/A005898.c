/*
 * A005898  Centered cube numbers: n^3 + (n+1)^3.
 * https://oeis.org/A005898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^3 + (n+1)^3 = (2*n+1)*(n^2+n+1)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t factor1 = 2 * nn + 1;
    seq_t factor2 = nn * nn + nn + 1;
    return factor1 * factor2;
}
