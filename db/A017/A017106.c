/*
 * A017106  a(n) = (8*n+3)^6.
 * https://oeis.org/A017106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 202, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 8*n + 3 */
    seq_t base = (seq_t)n * 8 + 3;

    /* raise base to the 6th power */
    seq_t b2 = base * base;           /* base^2 */
    seq_t b3 = b2 * base;             /* base^3 */
    seq_t b4 = b3 * base;             /* base^4 */
    seq_t b5 = b4 * base;             /* base^5 */
    seq_t b6 = b5 * base;             /* base^6 */
    return b6;
}
