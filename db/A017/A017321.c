/*
 * A017321  a(n) = (10*n + 4)^5.
 * https://oeis.org/A017321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 4)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute the base value. */
    seq_t base = (seq_t)n * 10 + 4;

    /* Compute the fifth power by repeated multiplication. */
    seq_t result = 1;
    /* multiply base five times */
    result *= base;  /* base^1 */
    result *= base;  /* base^2 */
    result *= base;  /* base^3 */
    result *= base;  /* base^4 */
    result *= base;  /* base^5 */

    return result;
}
