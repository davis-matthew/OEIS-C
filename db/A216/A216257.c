/*
 * A216257  a(n) = 840*n^2 - 23100*n + 86861.
 * https://oeis.org/A216257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 840*n^2 - 23100*n + 86861
 * Range:     i8 none, i16 none, i32 n <= 1000, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* n * (840*n - 23100) + 86861  */
    seq_t term = 840 * nn - (seq_t)23100;
    return nn * term + (seq_t)86861;
}
