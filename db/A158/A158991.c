/*
 * A158991  Numerator of Hermite(n, 4/7).
 * https://oeis.org/A158991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) - 8*a(n-1) + 98*(n-1)*a(n-2) = 0, a(0)=1, a(1)=8
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = (seq_t)98 * (seq_t)(i - 1);
        cur = (seq_t)8 * prev1 - factor * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
