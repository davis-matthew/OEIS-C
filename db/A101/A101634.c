/*
 * A101634  Subtract 1, multiply by 1, subtract 2, multiply by 2, etc.
 * https://oeis.org/A101634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=6; a(n+1)= n*a(n) - n^2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 6;
    if (n <= 1)
        return val;
    for (idx_t i = 1; i < n; ++i)
    {
        val = (seq_t)i * val - (seq_t)i * (seq_t)i;
    }
    return val;
}
