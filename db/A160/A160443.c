/*
 * A160443  Numerator of Hermite(n, 31/32).
 * https://oeis.org/A160443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 31*a(n-1) - 512*(n-1)*a(n-2), a(0)=1, a(1)=31
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;

    /* Iterative computation using the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)31;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t mult  = (seq_t)512 * (seq_t)(i - 1);   /* 512*(i-1) */
        seq_t term  = (seq_t)31 * prev1 - mult * prev2;
        prev2 = prev1;
        prev1 = term;
    }
    return prev1;
}
