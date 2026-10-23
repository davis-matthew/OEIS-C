/*
 * A159784  Numerator of Hermite(n, 19/21).
 * https://oeis.org/A159784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 21^n * Hermite(n, 19/21)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)38;
    const seq_t mult_441 = (seq_t)441;
    const seq_t factor_38 = (seq_t)38;
    seq_t prev2 = (seq_t)1;   /* b0 */
    seq_t prev1 = (seq_t)38;  /* b1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = (seq_t)2 * ((seq_t)(i - 1)) * mult_441 * prev2;
        seq_t cur  = factor_38 * prev1 - term;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
