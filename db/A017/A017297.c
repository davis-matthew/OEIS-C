/*
 * A017297  a(n) = (10*n + 2)^5.
 * https://oeis.org/A017297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 8, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 10*n + 2 */
    seq_t base = (seq_t)10 * n + (seq_t)2;

    /* compute base^5 by repeated multiplication */
    seq_t result = base;
    result *= base; /* base^2 */
    result *= base; /* base^3 */
    result *= base; /* base^4 */
    result *= base; /* base^5 */

    return result;
}
