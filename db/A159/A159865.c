/*
 * A159865  Numerator of Hermite(n, 3/23).
 * https://oeis.org/A159865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = 23^n * Hermite(n, 3/23)
   Recurrence derived from Hermite relation:
   a(0) = 1
   a(1) = 6
   a(n+1) = 6 * a(n) – 1058 * n * a(n‑1)   for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t curr   = prev1;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* recurrence: a(i) = 6*a(i-1) - 1058*(i-1)*a(i-2) */
        curr = prev1 * 6 - prev2 * 1058 * (seq_t)(i - 1);
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
