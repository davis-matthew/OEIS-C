/*
 * A158443  a(n) = 16*n^2 - 4.
 * https://oeis.org/A158443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 4
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 16 * n * n - 4, using seq_t for all intermediate results */
    return ((seq_t)16 * (seq_t)n * (seq_t)n) - (seq_t)4;
}
