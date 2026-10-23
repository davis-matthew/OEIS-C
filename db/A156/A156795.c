/*
 * A156795  a(n) = 81*n - 59.
 * https://oeis.org/A156795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n - 59
 * Range:     u8 n <= 3, u16 n <= 809, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 81*n - 59 in seq_t type, converting n first */
    return ((seq_t)81 * (seq_t)n) - 59;
}
