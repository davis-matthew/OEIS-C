/*
 * A213681  a(n) = a(n-1)^2/2 - 4 with a(0) = 6.
 * https://oeis.org/A213681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2/2 - 4, a(0) = 6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 6;
    seq_t val = 6;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * val / 2 - 4;
    }
    return val;
}
