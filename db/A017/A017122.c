/*
 * A017122  a(n) = (8*n + 4)^10.
 * https://oeis.org/A017122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 4)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10, u128 n <= 890,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base 8*n + 4 in seq_t */
    seq_t base = ((seq_t)8 * n) + 4;

    /* compute base^10 by repeated multiplication */
    seq_t result = 1;
    for (idx_t i = 0; i < 10; ++i) {
        result *= base;
    }

    return result;
}
