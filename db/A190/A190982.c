/*
 * A190982  a(n) = 9*a(n-1) - 5*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 5*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;   // a(n-2)
    seq_t prev1 = 1;   // a(n-1)
    seq_t cur = 0;     // a(n)

    for (idx_t i = 2; i <= n; ++i) {
        cur = 9 * prev1 - 5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
