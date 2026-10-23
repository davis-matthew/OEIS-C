/*
 * A181106  Largest odd number strictly less than a square.
 * https://oeis.org/A181106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^2 - 2^(n mod 2) (n>=1)
 * Range:     i8 n <= 11, i16 n <= 181, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    For even n: a(n) = n^2 - 1; */
/*    For odd  n: a(n) = n^2 - 2; */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n & (idx_t)1)          /* odd */
        return nn * nn - (seq_t)2;
    else                       /* even */
        return nn * nn - (seq_t)1;
}
