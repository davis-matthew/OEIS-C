/*
 * A218457  a(n) = 6*n^3 - 263*n^2 + 3469*n - 12841.
 * https://oeis.org/A218457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*n^3 - 263*n^2 + 3469*n - 12841
 * Range:     i8 none, i16 n <= 33, i32 n <= 724, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;          /* cast index to seq_t */
    seq_t n2 = nn * nn;    /* n^2 */
    seq_t n3 = n2 * nn;    /* n^3 */
    return 6 * n3 - 263 * n2 + 3469 * nn - 12841;
}
