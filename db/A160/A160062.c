/*
 * A160062  Numerator of Hermite(n, 17/25).
 * https://oeis.org/A160062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 17/25); Recurrence: a(0)=1, a(1)=34,
 *            a(n)=34*a(n-1)-1250*(n-1)*a(n-2) for n>=2
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;
    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)34;     /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = (seq_t)1250 * (seq_t)(i - 1) * a0;
        seq_t ai   = (seq_t)34 * a1 - term;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
