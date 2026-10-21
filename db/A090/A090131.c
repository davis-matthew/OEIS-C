/*
 * A090131  Expansion of (1+x)/(1 - 2*x + 2*x^2).
 * https://oeis.org/A090131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2); a(0) = 1, a(1) = 3
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)3; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t twoPrev1 = prev1 + prev1; /* 2*a(n-1) */
        seq_t twoPrev0 = prev2 + prev2; /* 2*a(n-2) */
        cur = twoPrev1 - twoPrev0;      /* a(n) = 2*a(n-1) - 2*a(n-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
