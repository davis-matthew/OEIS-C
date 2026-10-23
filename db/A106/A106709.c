/*
 * A106709  Expansion of g.f. -2*x/(1 - 5*x + 2*x^2).
 * https://oeis.org/A106709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2), a(0)=0, a(1)=-2
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 29, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-2;

    seq_t prev2 = (seq_t)0;        /* a(0) */
    seq_t prev1 = (seq_t)-2;       /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)5 - prev2 * (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
