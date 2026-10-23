/*
 * A187322  a(n) = floor(n/2) + floor(3*n/4).
 * https://oeis.org/A187322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + floor(3*n/4)
 * Range:     u8 n <= 205, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    /* floor(3*n/4) = 3*(N>>2) + floor(3*(N & 3)/4)  */
    return (N >> 1) + ((N >> 2) * 3 + (((N & 3) * 3) >> 2));
}
