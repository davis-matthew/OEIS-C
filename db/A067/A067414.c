/*
 * A067414  Seventh column of triangle A067410.
 * https://oeis.org/A067414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*14^(n-1) for n>=1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t val = (seq_t)8;
    for (idx_t i = 1; i < n; ++i)
        val *= (seq_t)14;
    return val;
}
