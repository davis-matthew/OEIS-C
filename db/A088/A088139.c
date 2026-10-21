/*
 * A088139  a(n) = 2*a(n-1) - 6*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A088139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 6*a(n-2), a(0)=0, a(1)=1
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 49, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;
    if (n == 1ULL) return (seq_t)1;

    seq_t prev2 = (seq_t)0; /* a(n-2) */
    seq_t prev1 = (seq_t)1; /* a(n-1) */
    seq_t cur;              /* a(n)  */

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)2 * prev1 - (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
