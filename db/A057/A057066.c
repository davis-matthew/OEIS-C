/*
 * A057066  a(n) = floor(4^4/n).
 * https://oeis.org/A057066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊256 / n⌋
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(256/n) using integral types that can hold 256 */
    return (seq_t)((idx_t)256 / n);
}
