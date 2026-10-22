/*
 * A006408  Number of nonseparable rooted toroidal maps with n + 3 edges and n
 *          + 1 vertices.
 * https://oeis.org/A006408
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*n - 28*n^2 - 45*n^3 + 20*n^4 + 33*n^5 + 8*n^6) / 360
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t n2 = m * m;
    seq_t n3 = n2 * m;
    seq_t n4 = n3 * m;
    seq_t n5 = n4 * m;
    seq_t n6 = n5 * m;
    
    return (12*m - 28*n2 - 45*n3 + 20*n4 + 33*n5 + 8*n6) / 360;
}
