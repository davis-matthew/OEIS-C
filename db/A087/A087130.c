/*
 * A087130  a(n) = 5*a(n-1)+a(n-2) for n>1, a(0)=2, a(1)=5.
 * https://oeis.org/A087130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)+a(n-2), a(0)=2, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)5;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)5 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
