/*
 * A156162  a(n) = 34*a(n-1)-a(n-2)-2312 for n > 2; a(1)=625, a(2)=18769.
 * https://oeis.org/A156162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34*a(n-1)-a(n-2)-2312 with a(1)=625, a(2)=18769
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1, undefined for n < 1 */
    if (n == 1) return 625;
    if (n == 2) return 18769;

    seq_t prev2 = 625;            /* a(1) */
    seq_t prev1 = 18769;          /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = 34 * prev1 - prev2 - 2312;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
