/*
 * A165748  a(n) = (8/9)*(2+7*(-8)^(n-1)).
 * https://oeis.org/A165748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165748
   Formula: a(0) = 1; a(n) = -8 * a(n-1) + 16  (n ≥ 1)
   (equivalent to a(n) = (8/9) * (2 + 7 * (-8)^(n-1)) )
*/
seq_t a(idx_t n)
{
    seq_t val = 1;                     /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        val = - ( (seq_t)8 * val ) + 16;
    }
    return val;
}
