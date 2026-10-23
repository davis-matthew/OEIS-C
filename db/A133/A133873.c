/*
 * A133873  n modulo 3 repeated 3 times.
 * https://oeis.org/A133873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + floor(n/3)) mod 3
 * Range:     u8 n <= 282, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n / 3;
    return (seq_t)(1 + m) % 3;
}
