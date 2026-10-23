/*
 * A176338  a(n) = 1 + (1-3^n)*a(n-1) for n >=1, a(0) = 0.
 * https://oeis.org/A176338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 + (1-3^n)*a(n-1), a(0) = 0
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 5, i64 n <= 8, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t pow_three = 1;   /* 3^0 */
    seq_t prev      = 0;   /* a(0)   */
    for (idx_t k = 1; k <= n; ++k) {
        pow_three *= (seq_t)3;                      /* 3^k */
        prev = 1 + ((1 - pow_three) * prev);       /* a(k) */
    }
    return prev;
}
