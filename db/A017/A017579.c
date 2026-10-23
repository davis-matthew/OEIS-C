/*
 * A017579  a(n) = (12*n + 4)^11.
 * https://oeis.org/A017579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 4)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 264,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base 12*n + 4 */
    seq_t base = ((seq_t)n * 12) + 4;
    /* raise base to the 11th power by repeated multiplication */
    seq_t result = 1;
    for (int i = 0; i < 11; ++i)
        result *= base;
    return result;
}
