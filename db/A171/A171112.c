/*
 * A171112  Duplicate of A171108.
 * https://oeis.org/A171112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (9*n^4 - 36*n^3 + 12*n^2 + 81*n - 66) / 2
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* value for n = 0 is outside the sequence, return 0 */
    seq_t nn = (seq_t)n;          /* cast the index to the sequence type */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */

    seq_t numer = 9 * n4 - 36 * n3 + 12 * n2 + 81 * nn - 66;
    return numer / 2;             /* integer division, numerator is always even */
}
