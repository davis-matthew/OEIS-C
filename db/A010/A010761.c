/*
 * A010761  a(n) = floor(n/2) + floor(n/3).
 * https://oeis.org/A010761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + floor(n/3)
 * Range:     u8 n <= 307, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)(n / 2) + (seq_t)(n / 3);
}
