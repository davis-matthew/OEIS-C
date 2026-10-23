/*
 * A112089  Erroneous version of A038119.
 * https://oeis.org/A112089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n-1)! for n>0, but a(4)=7; a(0)=1
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 5, i64 n <= 5, i128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)7;    /* erroneous value */
    idx_t m = n - 1;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i)
        result *= (seq_t)i;
    return result;
}
