/*
 * A017514  a(n) = (11*n + 10)^6.
 * https://oeis.org/A017514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 10)^6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 146, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base value */
    seq_t base = (seq_t)11 * (seq_t)n + (seq_t)10;

    /* Raise to the 6th power */
    seq_t result = base;
    for (int i = 1; i < 6; ++i)
        result *= base;

    return result;
}
