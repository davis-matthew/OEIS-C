/*
 * A161714  a(n) = 28*n + 1.
 * https://oeis.org/A161714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*n + 1
 * Range:     u8 n <= 9, u16 n <= 2340, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return 28 * (seq_t)n + 1;
}
