/*
 * A053743  Sum of numbers in range 10*n to 10*n+9.
 * https://oeis.org/A053743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n + 45
 * Range:     u8 n <= 2, u16 n <= 654, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)100 + (seq_t)45;
}
