/*
 * A157371  a(n) = (n+1)*(9-9*n+5*n^2-n^3).
 * https://oeis.org/A157371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+1)*(9-9*n+5*n^2-n^3)
 * Range:     i8 n <= 4, i16 n <= 14, i32 n <= 216, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)9;
    term -= (seq_t)9 * nn;
    term += (seq_t)5 * nn * nn;
    term -= nn * nn * nn;
    return (nn + (seq_t)1) * term;
}
