/*
 * A195744  a(n) = 15*2^(n+1) + 1.
 * https://oeis.org/A195744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15*2^(n+1) + 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)31;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)  /* a(n) = 2*a(n-1) - 1 */
    {
        val = val + val - 1;
    }
    return val;
}
