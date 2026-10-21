/*
 * A214067  a(n) = [(5/2)*[(5/2)*n]], where [ ] = floor.
 * https://oeis.org/A214067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5/2)*floor((5/2)*n))
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t m  = (5 * nn) / 2;  /* floor((5/2)*n) */
    return (5 * m) / 2;       /* floor((5/2)*floor((5/2)*n)) */
}
