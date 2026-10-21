/*
 * A090169  a(n) = floor( 3*n/2 ) + floor( 2*n/3 ).
 * https://oeis.org/A090169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊3*n/2⌋ + ⌊2*n/3⌋
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before multiplication to avoid truncation */
    seq_t m = (seq_t)n;
    seq_t part1 = (m * 3) / 2;   /* floor(3*n/2) */
    seq_t part2 = (m * 2) / 3;   /* floor(2*n/3) */
    return part1 + part2;
}
