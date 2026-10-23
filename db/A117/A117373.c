/*
 * A117373  Expansion of (1 - 3x)/(1 - x + x^2).
 * https://oeis.org/A117373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - a(n-2); a(0) = 1, a(1) = -2
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)-2;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 - prev2; /* a(i) = a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
