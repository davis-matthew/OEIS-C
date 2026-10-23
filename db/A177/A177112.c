/*
 * A177112  a(n) = 4*(10^n-7).
 * https://oeis.org/A177112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(10^n-7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;          /* compute 10^n */
    for (idx_t i = 0; i < n; ++i)
        pow10 *= 10;
    pow10 -= 7;               /* 10^n - 7 */
    pow10 *= 4;               /* 4 * (10^n - 7) */
    return pow10;
}
