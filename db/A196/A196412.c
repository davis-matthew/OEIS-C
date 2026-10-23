/*
 * A196412  a(n) = n! - n^5.
 * https://oeis.org/A196412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = n! - n^5
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* compute n^5 */
    seq_t pn  = (seq_t)n;     /* n */
    seq_t n2  = pn * pn;      /* n^2 */
    seq_t n4  = n2 * n2;      /* n^4 */
    seq_t n5  = n4 * pn;      /* n^5 */

    return fact - n5;
}
