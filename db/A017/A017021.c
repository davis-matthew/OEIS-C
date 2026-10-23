/*
 * A017021  a(n) = (7*n + 3)^5.
 * https://oeis.org/A017021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 3)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 11, u64 n <= 1018,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 7*n + 3, cast n to seq_t to avoid premature overflow */
    seq_t base = (seq_t)n * 7 + 3;
    /* compute base^5 by repeated multiplication */
    seq_t result = base * base; /* base^2 */
    result *= base;             /* base^3 */
    result *= base;             /* base^4 */
    result *= base;             /* base^5 */
    return result;
}
