/*
 * A228933  Optimal ascending continued fraction expansion of phi-1 = 1/phi =
 *          (sqrt(5)-1)/2.
 * https://oeis.org/A228933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, cofr
 * Formula:   a(1) = 2 a(2) = 4 a(3) = -18 a(n) = a(n-1)^2 - 2 for n > 3
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 6, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* values below the offset are irrelevant; return 0 for n==0 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)-18;

    /* start from a(3) and iterate the recurrence up to n */
    seq_t val = (seq_t)-18;
    for (idx_t i = 4; i <= n; ++i) {
        val = val * val - (seq_t)2;
    }
    return val;
}
