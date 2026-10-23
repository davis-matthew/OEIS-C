/*
 * A106393  Expansion of 1/(1 - 8*x + 17*x^2).
 * https://oeis.org/A106393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 8*a(n-1) - 17*a(n-2), a(0) = 1, a(1) = 8
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 14, i64 n <= 29, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)8 * prev1 - (seq_t)17 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
