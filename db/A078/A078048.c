/*
 * A078048  Expansion of (1-x)/(1+x+2*x^2-2*x^3).
 * https://oeis.org/A078048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) + 2*a(n-3) with initial values a(0) =
 *            1, a(1) = -2, a(2) = 0
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)0;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)-2;
    seq_t a2 = (seq_t)0;
    seq_t an;
    for (idx_t i = 3; i <= n; ++i) {
        an = -a2 - (seq_t)2 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
