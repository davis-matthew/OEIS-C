/*
 * A049389  a(n) = (n+8)!/8!.
 * https://oeis.org/A049389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+8)!/8!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* (i + 8) is promoted to seq_t before multiplication */
        result = result * ((seq_t)(i + 8));
    }
    return result;
}
