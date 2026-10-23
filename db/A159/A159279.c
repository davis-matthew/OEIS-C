/*
 * A159279  Numerator of Hermite(n, 9/10).
 * https://oeis.org/A159279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 5^n * Hermite(n, 9/10). Recurrence: a(0) = 1, a(1) = 9,
 *            and for n >= 2 a(n) = 9 * a(n-1) - 50 * (n-1) * a(n-2)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t a0 = (seq_t)1;  /* a(n-2) */
    seq_t a1 = (seq_t)9;  /* a(n-1) */
    seq_t an = 0;         /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        an = (9 * a1) - (50 * (seq_t)(i - 1) * a0);
        a0 = a1;
        a1 = an;
    }
    return a1;
}
