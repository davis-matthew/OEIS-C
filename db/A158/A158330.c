/*
 * A158330  a(n) = 484*n - 1.
 * https://oeis.org/A158330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 484*n - 1
 * Range:     u8 none, u16 n <= 135, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * 484) - 1;
}
