/*
 * A017215  a(n) = (9*n + 4)^7.
 * https://oeis.org/A017215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 4)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 62, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first, then compute the base. */
    seq_t base = (seq_t)n * 9 + 4;

    /* Compute base^7 multiplicatively to avoid intermediate overflows
       beyond the range of seq_t for typical builds. */
    seq_t result = base;          /* base^1   */
    result *= base;               /* base^2   */
    result *= base;               /* base^3   */
    result *= base;               /* base^4   */
    result *= base;               /* base^5   */
    result *= base;               /* base^6   */
    result *= base;               /* base^7   */

    return result;
}
