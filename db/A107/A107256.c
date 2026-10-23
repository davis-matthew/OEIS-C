/*
 * A107256  a(n) = n^6 - 63*n + 63, with n*a(n) + n*(n-1)*63 = n^7.
 * https://oeis.org/A107256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 - 63*n + 63
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the natural index to the sequence type once */
    seq_t nn = (seq_t)n;

    /* Compute n^6 carefully using seq_t arithmetic */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n6 = n3 * n3;           /* n^6 */

    /* Final result: n^6 - 63*n + 63 */
    return n6 - (seq_t)63 * nn + (seq_t)63;
}
