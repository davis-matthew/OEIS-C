/*
 * A017515  a(n) = (11*n + 10)^7.
 * https://oeis.org/A017515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 10)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 50, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 11*n + 10 */
    seq_t base = (seq_t)n * (seq_t)11 + (seq_t)10;

    /* compute base^7 by repeated multiplication */
    seq_t r = base;
    for (int i = 1; i < 7; ++i)
        r *= base;
    return r;
}
