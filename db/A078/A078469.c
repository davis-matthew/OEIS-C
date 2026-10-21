/*
 * A078469  Number of different compositions of the ladder graph L_n.
 * https://oeis.org/A078469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=12; a(n)=6*a(n-1)+a(n-2) for n>=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;
    seq_t prev2 = (seq_t)2;   /* a(1) */
    seq_t prev1 = (seq_t)12;  /* a(2) */
    seq_t cur;
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)6 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
