/*
 * A160063  Numerator of Hermite(n, 18/25).
 * https://oeis.org/A160063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 18/25) Recurrence: a(0)=1, a(1)=36,
 *            a(n+1)=36*a(n)-1250*n*a(n-1) for n≥1
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)36;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)36;  /* a(1) */

    for (idx_t i = 1; i < n; ++i)  /* compute a(i+1) */
    {
        seq_t tmp = (seq_t)36 * prev1 - (seq_t)1250 * (seq_t)i * prev2;
        prev2 = prev1;
        prev1 = tmp;
    }
    return prev1;
}
