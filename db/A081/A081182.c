/*
 * A081182  5th binomial transform of (0,1,0,2,0,4,0,8,0,16,...).
 * https://oeis.org/A081182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2) with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;              /* a(n-2) */
    seq_t prev1 = 1;              /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)10 * prev1 - (seq_t)23 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
