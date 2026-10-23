/*
 * A100775  a(n) = 97*n + 101.
 * https://oeis.org/A100775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 97*n + 101
 * Range:     u8 n <= 1, u16 n <= 674, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 97 + 101;
}
