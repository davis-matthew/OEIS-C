/*
 * A158965  Numerator of Hermite(n, 3/5).
 * https://oeis.org/A158965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 5^n * Hermite(n, 3/5) Recurrence: a(0) = 1 a(1) = 6 a(n)
 *            = 6 * a(n-1) - 50 * (n-1) * a(n-2) (for n ≥ 2)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)6;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 50*(i-1)*a(i-2) */
        cur = (seq_t)6 * prev1 - (seq_t)50 * (seq_t)(i - 1) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
