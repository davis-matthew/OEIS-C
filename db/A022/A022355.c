/*
 * A022355  Fibonacci sequence beginning 0, 21.
 * https://oeis.org/A022355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=21, a(n)=a(n-1)+a(n-2) for n>1 (Fibonacci sequence
 *            multiplied by 21)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)21;
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)21;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
