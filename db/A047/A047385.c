/*
 * A047385  Numbers that are congruent to {2, 5} mod 7.
 * https://oeis.org/A047385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 2 - floor(n/2)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t half = (seq_t)(n >> 1);
    return nn * 4 - (seq_t)2 - half;
}
