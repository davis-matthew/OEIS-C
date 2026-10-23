/*
 * A171372  a(n) = numerator of 1/(2*n)^2 - 1/(3*n)^2 for n > 0, a(0) = 1.
 * https://oeis.org/A171372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 1 if n mod 5 == 0 else 5
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 5 == 0) ? 1 : 5;
}
