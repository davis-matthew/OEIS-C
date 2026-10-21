/*
 * A085268  Integer part of the conversion from Fahrenheit to Celsius.
 * https://oeis.org/A085268
 *
 * Offset:    32    (a(32) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(5*(n-32)/9) (offset 32)
 * Range:     u8 n <= 357, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 32U) return 0;           /* behaviour below offset is irrelevant */
    return 5 * ((seq_t)n - 32) / 9;
}
