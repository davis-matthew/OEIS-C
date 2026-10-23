/*
 * A017505  a(n) = (11*n + 9)^9.
 * https://oeis.org/A017505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 9)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 11, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base (11*n + 9) using seq_t arithmetic */
    seq_t base = (seq_t)11 * n + (seq_t)9;

    /* raise base to the 9th power */
    seq_t result = 1;
    for (int i = 0; i < 9; ++i)
        result *= base;

    return result;
}
