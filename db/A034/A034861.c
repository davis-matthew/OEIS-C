/*
 * A034861  a(n) = n!*(3*n^2 - 15*n + 10)/6.
 * https://oeis.org/A034861
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n!*(3*n^2 - 15*n + 10)/6
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) {
        /* below the offset the value is arbitrary */
        return 0;
    }
    seq_t tn = (seq_t)n;
    seq_t poly = (3 * tn * tn) - 15 * tn + 10;
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    return fact * poly / 6;
}
