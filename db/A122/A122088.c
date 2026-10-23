/*
 * A122088  Add 10, subtract 5, add 10, subtract 5, ad infinitum.
 * https://oeis.org/A122088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2j+1) = 5j+1, a(2j) = 5j+6
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;              /* ensure arithmetic in seq_t type */
    return (m >> 1) * 5 + ((m & 1) ? 1 : 6);
}
