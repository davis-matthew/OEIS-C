/*
 * A121262  The characteristic function of the multiples of four.
 * https://oeis.org/A121262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = !(n%4)
 * Range:     u8 n <= 9556, u16 n <= 9556, u32 n <= 9556, u64 n <= 9556,
 *            u128 n <= 9556, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n % 4) == 0);
}
