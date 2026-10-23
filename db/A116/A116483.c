/*
 * A116483  Expansion of (1 + x) / (5*x^2 - 2*x + 1).
 * https://oeis.org/A116483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 5*a(n-2); a(0) = 1, a(1) = 3
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 26, i64 n <= 54, i128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)2 - a0 * (seq_t)5;
        a0 = a1;
        a1 = an;
    }
    return an;
}
