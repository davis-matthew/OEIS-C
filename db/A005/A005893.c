/*
 * A005893  Number of points on surface of tetrahedron; coordination sequence
 *          for sodalite net (equals 2*n^2+2 for n > 0).
 * https://oeis.org/A005893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n^2 - 0^n + 2
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t result = 2 * nn * nn + 2;
    
    if (n == 0) {
        result--;
    }
    
    return result;
}
