/*
 * A016926  a(n) = (6*n + 1)^6.
 * https://oeis.org/A016926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 1)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 6, u64 n <= 270, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 6*n + 1 */
    seq_t base = ((seq_t)6 * (seq_t)n) + (seq_t)1;

    /* compute base^6 by repeated multiplication */
    seq_t result = base;
    for (idx_t i = 1; i < 6; ++i)
    {
        result *= base;
    }

    return result;
}
