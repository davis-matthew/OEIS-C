/*
 * A017561  a(n) = (12*n + 3)^5.
 * https://oeis.org/A017561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 6, u64 n <= 594, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base value 12*n + 3 */
    seq_t base = (seq_t)12 * n + (seq_t)3;

    /* Raise base to the 5th power using repeated multiplication */
    seq_t result = base;          /* base^1 */
    result *= base;               /* base^2 */
    result *= base;               /* base^3 */
    result *= base;               /* base^4 */
    result *= base;               /* base^5 */
    return result;
}
