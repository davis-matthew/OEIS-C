/*
 * A134632  a(n) = 5*n^5 + 3*n^3 - 2*n^2. Coefficients and exponents are the
 *          prime numbers in decreasing order.
 * https://oeis.org/A134632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^5 + 3*n^3 - 2*n^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t xn = (seq_t)n;       /* cast index to the sequence type */
    seq_t n2 = xn * xn;        /* n^2 */
    seq_t n3 = n2 * xn;        /* n^3 */
    seq_t n5 = n3 * n2;        /* n^5 = n^3 * n^2 */

    seq_t term5 = n5 * 5;      /* 5*n^5 */
    seq_t term3 = n3 * 3;      /* 3*n^3 */
    seq_t term2 = n2 * 2;      /* 2*n^2 */

    return term5 + term3 - term2;
}
