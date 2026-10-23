/*
 * A017190  a(n) = (9*n + 2)^6.
 * https://oeis.org/A017190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 180, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid mixed-type multiplication */
    seq_t base = (seq_t)9 * (seq_t)n + 2;

    /* Compute base^6 by successive multiplication */
    seq_t result = base * base;        /* base^2 */
    result = result * base;            /* base^3 */
    result = result * base;            /* base^4 */
    result = result * base;            /* base^5 */
    result = result * base;            /* base^6 */

    return result;
}
