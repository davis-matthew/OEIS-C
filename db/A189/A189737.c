/*
 * A189737  a(1)=3, a(2)=3, a(n)=3*a(n-1) + 3*a(n-2).
 * https://oeis.org/A189737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2), with a(1)=3, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 3;
    seq_t prev2 = 3;      /* a(1) */
    seq_t prev1 = 3;      /* a(2) */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i) {
        curr = (prev1 + prev2) * 3;   /* 3*(a(n-1)+a(n-2)) */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
