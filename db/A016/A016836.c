/*
 * A016836  a(n) = (4*n + 2)^12.
 * https://oeis.org/A016836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 9, u128 n <= 405,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 4*n + 2 */
    seq_t base = ((seq_t)4 * (seq_t)n) + 2;
    seq_t result = 1;
    /* raise base to the 12th power by repeated multiplication */
    for (int i = 0; i < 12; ++i)
        result *= base;
    return result;
}
