/*
 * A160334  Numerator of Hermite(n, 23/31).
 * https://oeis.org/A160334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 31^n * Hermite(n, 23/31). Recurrence: a(0) = 1, a(1) =
 *            46, a(n) = 46 * a(n-1) - 1922 * (n-1) * a(n-2) (n ≥ 2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)46;

    seq_t a0 = (seq_t)1;            /* a(0) */
    seq_t a1 = (seq_t)46;           /* a(1) */
    seq_t ai;                       /* a(i) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t factor = (seq_t)1922 * (seq_t)(i - 1);   /* 1922*(i-1) */
        ai = (seq_t)46 * a1 - factor * a0;            /* recurrence */
        a0 = a1;                                       /* shift for next step */
        a1 = ai;
    }

    return a1;   /* a(n) is stored in a1 after the loop */
}
