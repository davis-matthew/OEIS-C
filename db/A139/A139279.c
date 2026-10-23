/*
 * A139279  a(n) = 40*n - 32.
 * https://oeis.org/A139279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*n - 32
 * Range:     u8 n <= 7, u16 n <= 1639, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) = 8 */
    return ((seq_t)n * (seq_t)40U) - (seq_t)32U;
}
