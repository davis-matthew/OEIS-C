/*
 * A017029  a(n) = 7*n + 4.
 * https://oeis.org/A017029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n + 4
 * Range:     u8 n <= 35, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * 7) + 4;
}
