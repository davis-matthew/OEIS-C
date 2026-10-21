/*
 * A238477  a(n) = 32*n - 27 for n >= 1. Second column of triangle A238475.
 * https://oeis.org/A238477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n - 27
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offsets start at 1; behavior for n < 1 is undefined. */
    return (seq_t)32 * (seq_t)n - (seq_t)27;
}
