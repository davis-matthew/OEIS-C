/*
 * A167554  The second left hand column of triangle A167552.
 * https://oeis.org/A167554
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2*n^3 - 15*n^2 + 19*n - 6)/6
 * Range:     i8 n <= 10, i16 n <= 48, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t res = ((seq_t)2 * nn * nn * nn
                 - (seq_t)15 * nn * nn
                 + (seq_t)19 * nn
                 - (seq_t)6) / (seq_t)6;
    return res;
}
