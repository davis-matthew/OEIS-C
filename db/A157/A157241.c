/*
 * A157241  Expansion of x / ((1-x)*(4*x^2-2*x+1)).
 * https://oeis.org/A157241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=0, a(1)=1, a(2)=3, a(n) = 3*a(n-1) - 6*a(n-2) + 4*a(n-3)
 *            for n > 2
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 31, i64 n <= 64, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a0 = (seq_t)0;   /* a(n-3)   */
    seq_t a1 = (seq_t)1;   /* a(n-2)   */
    seq_t a2 = (seq_t)3;   /* a(n-1)   */
    seq_t an = a2;         /* will hold a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)3 * a2 - (seq_t)6 * a1 + (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
