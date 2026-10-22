/*
 * A006221  From Apery continued fraction for zeta(3):
 *          zeta(3)=6/(5-1^6/(117-2^6/(535-3^6/(1463...)))).
 * https://oeis.org/A006221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*n^3 + 51*n^2 + 27*n + 5
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 501, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return 34*n3 + 51*n2 + 27*nn + 5;
}
