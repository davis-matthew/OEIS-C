/*
 * A079979  Characteristic function of multiples of six.
 * https://oeis.org/A079979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 6 == 0, otherwise 0
 * Range:     u8 n <= 65538, u16 n <= 65538, u32 n <= 65538, u64 n <= 65538,
 *            u128 n <= 65538, big any n
 * Validated: standard, pass, 137 terms through n = 9200 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n % 6) == 0);
}
