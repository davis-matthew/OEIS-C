/*
 * A014832  a(1)=1; for n>1, a(n) = 9*a(n-1) + n.
 * https://oeis.org/A014832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = 9*a(n-1) + n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    if (n <= 1)
        return val;
    for (idx_t i = 2; i <= n; ++i)
    {
        val = 9 * val + (seq_t)i;
    }
    return val;
}
