/*
 * A239123  a(n) = 128*n - 107 for n >= 1. Third column of triangle A238475.
 * https://oeis.org/A239123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n - 107, n >= 1
 * Range:     u8 n <= 2, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 128*n - 107 using seq_t arithmetic */
    seq_t result = (seq_t)n * (seq_t)128 - (seq_t)107;
    return result;
}
