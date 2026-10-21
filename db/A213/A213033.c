/*
 * A213033  n*[n/2]*[n/3], where [] = floor.
 * https://oeis.org/A213033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*[n/2]*[n/3]
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t n2 = n / 2;
    idx_t n3 = n / 3;
    return (seq_t)n * (seq_t)n2 * (seq_t)n3;
}
