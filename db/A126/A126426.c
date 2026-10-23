/*
 * A126426  a(n) = n^5 - n - 1.
 * https://oeis.org/A126426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^5 - n - 1
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 73, i64 n <= 6208,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;     /* n^2 */
    seq_t n3 = n2 * s;    /* n^3 */
    seq_t n5 = n2 * n3;   /* n^5 = n^2 * n^3 */
    return n5 - s - 1;
}
