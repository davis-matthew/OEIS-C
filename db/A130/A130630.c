/*
 * A130630  Periodic sequence with period 1 1 1 1 1 0 0 0 0.
 * https://oeis.org/A130630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 if n mod 9 < 5, otherwise 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 9;
    return (seq_t)(r < 5);
}
