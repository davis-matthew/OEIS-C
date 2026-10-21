/*
 * A057358  a(n) = floor(4*n/7).
 * https://oeis.org/A057358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(4*n/7)
 * Range:     u8 n <= 447, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 tmp = (unsigned __int128)n * 4;
    tmp /= 7;
    return (seq_t)tmp;
}
