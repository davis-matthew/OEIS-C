/*
 * A017637  a(n) = (12*n + 9)^9.
 * https://oeis.org/A017637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 9)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10, u128 n <= 1591,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 12*n + 9 */
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)9;
    /* compute base^9 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 9; ++i)
    {
        result *= base;
    }
    return result;
}
