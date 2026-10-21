/*
 * A072261  a(n) = 4*a(n-1) + 1, a(1)=7.
 * https://oeis.org/A072261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    seq_t res = 7;
    for (idx_t i = 2; i <= n; ++i)
    {
        res = res * 4 + 1;
    }
    return res;
}
