/*
 * A106392  Expansion of 1/(1 - 6*x + 10*x^2).
 * https://oeis.org/A106392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2), a(0) = 1, a(1) = 6
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 19, i64 n <= 38, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 10*a(i-2) */
        cur = (seq_t)6 * prev1 - (seq_t)10 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
