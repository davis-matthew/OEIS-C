/*
 * A017507  a(n) = (11*n + 9)^11.
 * https://oeis.org/A017507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 9)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 288,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (11*n + 9) as a seq_t value */
    seq_t base = (seq_t)11 * (seq_t)n + 9;

    /* Raise base to the 11th power */
    seq_t result = 1;
    for (idx_t i = 0; i < (idx_t)11; ++i)
        result *= base;

    return result;
}
