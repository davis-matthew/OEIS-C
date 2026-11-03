/*
 * A079998  The characteristic function of the multiples of five.
 * https://oeis.org/A079998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 5 == 0, else 0
 * Range:     u8 n <= 16385, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 122 terms through n = 9208 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 5 == 0) ? (seq_t)1 : (seq_t)0;
}
