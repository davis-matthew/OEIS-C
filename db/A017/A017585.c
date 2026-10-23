/*
 * A017585  a(n) = (12*n + 5)^5.
 * https://oeis.org/A017585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 5)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 593, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base (12*n + 5) using seq_t arithmetic */
    seq_t base = (seq_t)n * (seq_t)12 + (seq_t)5;

    /* compute base^5 by repeated multiplication */
    seq_t r = base;
    r *= base;  /* base^2 */
    r *= base;  /* base^3 */
    r *= base;  /* base^4 */
    r *= base;  /* base^5 */

    return r;
}
