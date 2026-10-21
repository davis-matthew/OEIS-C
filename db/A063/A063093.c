/*
 * A063093  Dimension of the space of weight 2n cusp forms for Gamma_0( 25 ).
 * https://oeis.org/A063093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n-1) for even n, 5*n-6 for odd n (n>1), a(1)=0
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    seq_t N = (seq_t)n;
    /* even n: 5*(n-1) = 5*n - 5
       odd  n: 5*n - 6 */
    if ((n & 1) == 0) {
        return 5 * N - 5;
    } else {
        return 5 * N - 6;
    }
}
