/*
 * A105280  a(0)=0; a(n) = 11*a(n-1) + 11.
 * https://oeis.org/A105280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 11*a(n-1) + 11, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        result = result * (seq_t)11 + (seq_t)11;
    }
    return result;
}
