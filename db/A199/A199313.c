/*
 * A199313  a(n) = 9*5^n+1.
 * https://oeis.org/A199313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*5^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow *= 5;
    }
    pow = pow * 9 + 1;
    return pow;
}
