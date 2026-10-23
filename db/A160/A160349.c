/*
 * A160349  Numerator of Hermite(n, 30/31).
 * https://oeis.org/A160349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of Hermite polynomial H_n(30/31) Recurrence:
 *            a(0) = 1 a(1) = 60 a(n) = 60 * a(n-1) - 1922 * (n-1) * a(n-2) (n
 *            ≥ 2) This follows from H_{n+1}(x) = 2x H_n(x) - 2n H_{n-1}(x)
 *            with x = 30/31
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (!n) return (seq_t)1;          /* a(0) */
    if (n == 1) return (seq_t)60;     /* a(1) */

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)60; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* term = 1922 * (i-1) * a(i-2) */
        seq_t term = ((seq_t)1922 * (seq_t)(i - 1)) * prev2;
        cur = ((seq_t)60 * prev1) - term;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
