/*
 * A160069  Numerator of Hermite(n, 1/26).
 * https://oeis.org/A160069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n, 1/26)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n)=a(n-1)-338*(n-1)*a(n-2) for n>=2 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;
    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t tmp = prev2 * (seq_t)(i - 1);
        tmp = tmp * (seq_t)338;
        seq_t curr = prev1 - tmp;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
