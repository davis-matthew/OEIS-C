/*
 * A128470  a(n) = 30*n + 1.
 * https://oeis.org/A128470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*n + 1
 * Range:     u8 n <= 8, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)30 * (seq_t)n + (seq_t)1;
}
