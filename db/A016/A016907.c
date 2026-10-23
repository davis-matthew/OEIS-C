/*
 * A016907  a(n) = (5*n + 4)^11.
 * https://oeis.org/A016907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10, u128 n <= 635,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 5*n + 4 */
    seq_t base = (seq_t)5 * (seq_t)n + (seq_t)4;

    /* compute base^11 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 11; ++i)
        result *= base;

    return result;
}
