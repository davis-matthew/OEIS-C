/*
 * A168328  a(n) = 6 * floor( n/2 ).
 * https://oeis.org/A168328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 6 * floor(n/2)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) for unsigned n is just a right shift by 1 */
    seq_t half = (seq_t)(n >> 1);
    return half * 6;
}
