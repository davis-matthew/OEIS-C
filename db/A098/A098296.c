/*
 * A098296  Member r=11 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - a(n-2) + 2, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    seq_t cur   = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = 9 * prev1 - prev2 + 2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
