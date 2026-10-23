/*
 * A014900  a(1)=1, a(n) = 17*a(n-1) + n.
 * https://oeis.org/A014900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) + n with a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t val = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * (seq_t)17 + (seq_t)i;
    }
    return val;
}
