/*
 * A063221  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          57 ).
 * https://oeis.org/A063221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n - 2 if n is even (n > 1) * a(n) = 6*n - 4 if n is odd
 *            (n > 1) * a(1) = 3 * This follows from a(n) = 2*A007310(n) and
 *            A007310(n) = 3*n - (n%2==0?1:2)
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)3;
    }
    /* Cast the index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Subtract the appropriate constant depending on the parity of n */
    if ((n & (idx_t)1) == 0) /* even */
        return 6 * nn - (seq_t)2;
    else                     /* odd */
        return 6 * nn - (seq_t)4;
}
