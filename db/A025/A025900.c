/*
 * A025900  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A025900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n^2+12*n+12)/12) - floor((n^2+3*n+4)/7) -
 *            floor((3*n^2+6*n+5)/11)
 * Range:     u8 n <= 472, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t term1 = (5*N*N + 12*N + 12) / 12;
    seq_t term2 = (N*N + 3*N + 4) / 7;
    seq_t term3 = (3*N*N + 6*N + 5) / 11;
    return term1 - term2 - term3;
}
