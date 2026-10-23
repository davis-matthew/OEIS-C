/*
 * A157363  a(n) = 686*n - 14.
 * https://oeis.org/A157363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 686*n - 14
 * Range:     u8 none, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the linear formula; cast n to seq_t first */
    return (seq_t)n * 686 - 14;
}
