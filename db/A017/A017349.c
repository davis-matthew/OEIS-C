/*
 * A017349  a(n) = (10*n + 6)^9.
 * https://oeis.org/A017349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 6)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 13, u128 n <= 1910,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 10*n + 6 */
    seq_t base   = (seq_t)n;
    base *= (seq_t)10;
    base += (seq_t)6;

    /* compute base^9 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 9; ++i)
        result *= base;

    return result;
}
