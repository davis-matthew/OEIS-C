/*
 * A105752  Expansion of e.g.f. cos(i*log(1 + x)), i = sqrt(-1).
 * https://oeis.org/A105752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = 0, a(n) = (-1)^n * n! / 2 for n > 1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t sign = ((n & 1) == 0) ? (seq_t)1 : (seq_t)-1;
    return sign * (fact / (seq_t)2);
}
