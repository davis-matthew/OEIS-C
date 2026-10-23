/*
 * A158225  a(n) = 196*n - 1.
 * https://oeis.org/A158225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 196*n - 1
 * Range:     u8 n <= 1, u16 n <= 334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)196 * (seq_t)n - (seq_t)1;
}
