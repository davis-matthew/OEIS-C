/*
 * A119413  a(n) = 16*n - 12.
 * https://oeis.org/A119413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 16*n - 12
 * Range:     u8 n <= 16, u16 n <= 4096, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 16 - 12;
}
