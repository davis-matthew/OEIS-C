/*
 * A025170  Expansion of g.f.: 1/(1 + 2*x + 9*x^2).
 * https://oeis.org/A025170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(2*a(n-1) + 9*a(n-2)), a(0)=1, a(1)=-2
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 40, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;

    seq_t a0 = (seq_t)1;      /* a(n-2) */
    seq_t a1 = (seq_t)-2;     /* a(n-1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = -( (seq_t)2 * a1 + (seq_t)9 * a0 );
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
