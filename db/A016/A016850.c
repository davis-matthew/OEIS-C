/*
 * A016850  a(n) = (5*n)^2.
 * https://oeis.org/A016850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n*n
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 800, u64 n <= 800,
 *            u128 n <= 800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)n * (seq_t)25;
}
