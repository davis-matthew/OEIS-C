/*
 * A152016  a(n) = n^4 - n^3 - n^2 - n.
 * https://oeis.org/A152016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4 - n^3 - n^2 - n
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 215, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t n2 = x * x;          /* n^2   */
    seq_t n3 = n2 * x;         /* n^3   */
    seq_t n4 = n3 * x;         /* n^4   */
    return n4 - n3 - n2 - x;   /* n^4 - n^3 - n^2 - n */
}
