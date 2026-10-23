/*
 * A116484  Expansion of (-1+3*x)/(5*x^2 + 1 - 2*x).
 * https://oeis.org/A116484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = -1, a(1) = 1, a(n) = 2*a(n-1) - 5*a(n-2) for n ≥ 2
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 26, i64 n <= 54, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    seq_t a0 = -1;
    seq_t a1 = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = 2 * a1 - 5 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
