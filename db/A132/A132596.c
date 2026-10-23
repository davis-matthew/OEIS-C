/*
 * A132596  X-values of solutions to the equation X*(X + 1) - 6*Y^2 = 0.
 * https://oeis.org/A132596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2; a(n)=10*a(n-1)-a(n-2)+4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;
    seq_t prev2 = 0;
    seq_t prev1 = 2;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 10 * prev1 - prev2 + 4;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
