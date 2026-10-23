/*
 * A019428  Continued fraction for tan(1/5).
 * https://oeis.org/A019428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = (-1 + 3*(-1)^n - 5*(-1 + (-1)^n)*n)/2 for n > 1 * with
 *            a(0) = 0 and a(1) = 4
 * Range:     u8 n <= 52, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 0 */
        return 0;
    if (n == 1)               /* a(1) = 4 */
        return 4;

    /* For n >= 2 : even indices give 1, odd indices give 5*n - 2 */
    if ((n & 1) == 0)         /* n is even */
        return 1;
    else                      /* n is odd */
        return (seq_t)5 * (seq_t)n - 2;
}
