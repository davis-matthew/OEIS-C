/*
 * A152015  a(n) = n^3 - n^2 - n.
 * https://oeis.org/A152015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - n^2 - n
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1290, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;   /* n^2 */
    seq_t nn3 = nn2 * nn;  /* n^3 */
    return nn3 - nn2 - nn;
}
