/*
 * A145919  A000332(n) = a(n)*(3*a(n) - 1)/2.
 * https://oeis.org/A145919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n % 3 == 0) ? n*(3-n)/6 : (n-1)*(n-2)/6
 * Range:     i8 n <= 29, i16 n <= 444, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0) {
        seq_t nn = (seq_t)n;
        return nn * (((seq_t)3 - nn)) / ((seq_t)6);
    } else {
        seq_t nn = (seq_t)n;
        return (nn - (seq_t)1) * (nn - (seq_t)2) / ((seq_t)6);
    }
}
