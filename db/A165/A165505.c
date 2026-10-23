/*
 * A165505  a(0)=1, a(1)=7, a(n) = 42*a(n-2) - a(n-1).
 * https://oeis.org/A165505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 42*a(n-2) - a(n-1); a(0) = 1; a(1) = 7
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 23, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prevprev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)7;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)42 * prevprev - prev;  /* a(i) = 42*a(i-2) - a(i-1) */
        prevprev = prev;
        prev     = cur;
    }
    return prev;
}
