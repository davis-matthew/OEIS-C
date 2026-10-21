/*
 * A084170  a(n) = (5*2^n + (-1)^n - 3)/3.
 * https://oeis.org/A084170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2, a(0)=1, a(1)=2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + prev2 * (seq_t)2 + (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
