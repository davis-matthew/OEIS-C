/*
 * A141354  Expansion of (1-5*x-x^2+x^3)/((1+x)*(1-x)^3).
 * https://oeis.org/A141354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((-1)^n - 2*n^2 - 4*n + 1)/2
 * Range:     i8 n <= 7, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* (-1)^n : 1 for even n, -1 for odd n */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;

    /* Compute the numerator:  sign - 2*n^2 - 4*n + 1 */
    seq_t numerator = sign - 2 * nn * nn - 4 * nn + 1;

    /* The numerator is always even, so final division is exact */
    return numerator / 2;
}
