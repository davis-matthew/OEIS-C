/*
 * A003453  Number of nonequivalent dissections of an n-gon into 3 polygons by
 *          nonintersecting diagonals up to rotation and reflection.
 * https://oeis.org/A003453
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n^3 - 6*n^2 - 23*n + 63 + 3*(n-5)*(-1)^n) / 48
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    
    seq_t result = (seq_t)2 * n3 - (seq_t)6 * n2 - (seq_t)23 * nn + (seq_t)63;
    
    seq_t sign_factor;
    if (n & 1) {
        sign_factor = -(seq_t)3 * (nn - (seq_t)5);
    } else {
        sign_factor = (seq_t)3 * (nn - (seq_t)5);
    }
    
    result = result + sign_factor;
    
    return result / (seq_t)48;
}
