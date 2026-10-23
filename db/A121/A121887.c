/*
 * A121887  a(n) = (n^5 - 133*n^4 + 6729*n^3 - 158379*n^2 + 1720294*n -
 *          6823316)/4.
 * https://oeis.org/A121887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^5 - 133*n^4 + 6729*n^3 - 158379*n^2 + 1720294*n -
 *            6823316)/4
 * Range:     i8 none, i16 none, i32 n <= 123, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = (seq_t)n;
    seq_t t2 = t * t;           /* n^2  */
    seq_t t3 = t2 * t;          /* n^3  */
    seq_t t4 = t3 * t;          /* n^4  */
    seq_t t5 = t4 * t;          /* n^5  */

    seq_t val = t5
                - (seq_t)133 * t4
                + (seq_t)6729 * t3
                - (seq_t)158379 * t2
                + (seq_t)1720294 * t
                - (seq_t)6823316;

    return val / (seq_t)4;
}
