/*
 * A065923  Bessel polynomial y_n(-3).
 * https://oeis.org/A065923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = -2,
   a(n) = -(6*n-3)*a(n-1) + a(n-2)   for n >= 2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;

    seq_t prev2 = (seq_t)1;       /* a(0) */
    seq_t prev1 = (seq_t)-2;      /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = (seq_t)i * (seq_t)6 - (seq_t)3;   /* (6*i-3) as seq_t */
        cur = -(factor * prev1) + prev2;                 /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
