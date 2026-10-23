/*
 * A165230  a(0)=1, a(1)=7, a(n)=14*a(n-1)-7*a(n-2) for n>1 .
 * https://oeis.org/A165230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=7, a(n)=14*a(n-1)-7*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
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
        cur = prev1 * (seq_t)14 - prev2 * (seq_t)7;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
