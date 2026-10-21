/*
 * A063369  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          96 ).
 * https://oeis.org/A063369
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(2) = -1; a(n) = 108*n-226 for n > 2
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)2)
        return (seq_t)-1;
    seq_t tn = (seq_t)n;
    return 108 * tn - 226;
}
