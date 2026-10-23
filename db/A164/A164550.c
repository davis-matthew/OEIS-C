/*
 * A164550  a(n) = 6*a(n-1) - 3*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A164550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2); a(0)=1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)7;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)6U - prev2 * (seq_t)3U;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
