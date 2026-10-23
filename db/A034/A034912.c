/*
 * A034912  One sixth of octo-factorial numbers.
 * https://oeis.org/A034912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * (8*n - 2), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        result *= (seq_t)(8 * i - 2);
    }
    return result;
}
