/*
 * A199021  a(n) = (5*11^n - 1)/2.
 * https://oeis.org/A199021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 5, with a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 2;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        result = result * 11 + 5;
    }
    return result;
}
