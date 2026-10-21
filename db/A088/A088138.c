/*
 * A088138  Generalized Gaussian Fibonacci integers.
 * https://oeis.org/A088138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   * a(0) = 0, a(1) = 1, a(2) = 2; * a(n) = 2*a(n-1) - 4*a(n-2) for
 *            n >= 3
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 31, i64 n <= 64, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t prev2 = (seq_t)0;  /* a(n-2) */
    seq_t prev1 = (seq_t)1;  /* a(n-1) */
    seq_t curr;              /* current a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)(2 * prev1 - 4 * prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
