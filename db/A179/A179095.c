/*
 * A179095  Rectified 5-cell numbers: the coefficient of x^{2n-2} in (1+x+x^2+
 *          ... + x^{n-1})^5.
 * https://oeis.org/A179095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(11*n^3+6*n^2+n+6)/24
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 172, u64 n <= 261,
 *            u128 n <= 261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the zero case directly */
    if (n == 0)
        return 0;

    /* Promote index to seq_t for all calculations */
    seq_t nn  = (seq_t)n;      /* n          */
    seq_t n2  = nn * nn;       /* n^2        */
    seq_t n3  = n2 * nn;       /* n^3        */

    /* Compute the polynomial part: 11*n^3 + 6*n^2 + n + 6 */
    seq_t term = 11 * n3 + 6 * n2 + nn + 6;

    /* Final result: n * term / 24 */
    return (nn * term) / 24;
}
