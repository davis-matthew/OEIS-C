/*
 * A067420  Fifth column of triangle A067417.
 * https://oeis.org/A067420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * 15^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    seq_t res = (seq_t)7;
    for (idx_t i = 2; i <= n; ++i)
    {
        res *= (seq_t)15;
    }
    return res;
}
