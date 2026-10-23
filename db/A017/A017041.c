/*
 * A017041  a(n) = 7*n + 5.
 * https://oeis.org/A017041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n + 5
 * Range:     u8 n <= 35, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 7*n + 5 in seq_t, using only integer operations */
    return ((seq_t)n * 7) + 5;
}
