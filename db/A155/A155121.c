/*
 * A155121  a(n) = 2*n*(1 + n + n^2 + n^3) - 3.
 * https://oeis.org/A155121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n*(1 + n + n^2 + n^3) - 3
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 180, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;          /* n^2 */
    seq_t n3   = n2 * nn;          /* n^3 */
    seq_t sum  = (seq_t)1 + nn + n2 + n3;  /* 1 + n + n^2 + n^3 */
    seq_t result = ((seq_t)2 * nn * sum) - (seq_t)3;
    return result;
}
