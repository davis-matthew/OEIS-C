/*
 * A153260  a(n) = n^3 - 3*(n+3)^2.
 * https://oeis.org/A153260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - 3*(n+3)^2
 * Range:     i8 n <= 7, i16 n <= 33, i32 n <= 1291, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x      = (seq_t)n;
    seq_t x2     = x * x;         /* n^2 */
    seq_t term1  = x2 * x;        /* n^3 */
    seq_t x3     = x + (seq_t)3;  /* n+3 */
    seq_t term2  = x3 * x3;       /* (n+3)^2 */
    return term1 - (seq_t)3 * term2;
}
