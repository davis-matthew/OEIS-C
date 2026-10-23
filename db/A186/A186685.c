/*
 * A186685  Total number of n-digit numbers requiring 19 positive biquadrates
 *          in their representation as sum of biquadrates.
 * https://oeis.org/A186685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 0 for n >= 4; a(1) = 0; a(2) = 1; a(3) = 6
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)6;
    return (seq_t)0;
}
