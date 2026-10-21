/*
 * A078343  a(0) = -1, a(1) = 2; a(n) = 2*a(n-1) + a(n-2).
 * https://oeis.org/A078343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1, a(1) = 2; a(n) = 2*a(n-1) + a(n-2)
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 50, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)-1;   /* a(0) */
    seq_t prev1 = (seq_t)2;    /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)2 * prev1 + prev2;  /* a(i) = 2*a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
