/*
 * A076314  a(n) = floor(n/10) + (n mod 10).
 * https://oeis.org/A076314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/10) + (n mod 10)
 * Range:     u8 n <= 2478, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n / 10) + (n % 10));
}
