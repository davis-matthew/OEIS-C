/*
 * A208826  Number of 5-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Piecewise polynomial * a(n) = (23*n^4 + 46*n^3 + 58*n^2 + 44*n +
 *            24)/24 for n even * a(n) = (23*n^4 + 46*n^3 + 58*n^2 + 26*n +
 *            15)/24 for n odd
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 119, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t numerator;

    if ((n & 1) == 0) {
        numerator = 23 * N4 + 46 * N3 + 58 * N2 + 44 * N + 24;
    } else {
        numerator = 23 * N4 + 46 * N3 + 58 * N2 + 26 * N + 15;
    }

    return numerator / (seq_t)24;
}
