/*
 * A001847  Crystal ball sequence for 5-dimensional cubic lattice.
 * https://oeis.org/A001847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^5 + 10*n^4 + 40*n^3 + 50*n^2 + 46*n + 15) / 15
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 108, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t result = 4;
    result = result * nn + 10;
    result = result * nn + 40;
    result = result * nn + 50;
    result = result * nn + 46;
    result = result * nn + 15;
    return result / 15;
}
