/*
 * A029699  Number of words of length 4 in the n(n-1)/2 transpositions of S[ n
 *          ] equivalent to the identity.
 * https://oeis.org/A029699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(3*n^2+n-12)/4
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn - 1);
    term = term * (3 * nn * nn + nn - 12);
    term /= 4;
    return term;
}
