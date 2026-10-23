/*
 * A158272  a(n) = 324*n + 1.
 * https://oeis.org/A158272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 324*n + 1
 * Range:     u8 none, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 324*n in seq_t arithmetic to avoid intermediate overflow */
    return ((seq_t)n) * 324 + 1;
}
