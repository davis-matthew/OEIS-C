/*
 * A157666  a(n) = 19683*n - 13716.
 * https://oeis.org/A157666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19683*n - 13716
 * Range:     u8 none, u16 n <= 4, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)19683 - (seq_t)13716;
}
