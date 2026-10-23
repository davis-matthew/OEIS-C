/*
 * A016939  a(n) = (6n+2)^7.
 * https://oeis.org/A016939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6n+2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 93, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base 6n+2 in the widest available type */
    seq_t base = (seq_t)n * 6 + 2;

    /* Raise it to the 7th power by repeated multiplication */
    seq_t result = base;     /* base^1 */
    result *= base;         /* base^2 */
    result *= base;         /* base^3 */
    result *= base;         /* base^4 */
    result *= base;         /* base^5 */
    result *= base;         /* base^6 */
    result *= base;         /* base^7 (final) */

    return result;
}
