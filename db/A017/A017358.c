/*
 * A017358  a(n) = (10*n + 7)^6.
 * https://oeis.org/A017358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^6
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 161, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 10*n + 7 */
    seq_t base = (seq_t)n * (seq_t)10 + (seq_t)7;

    /* compute base^6 by repeated multiplication */
    seq_t t1 = base * base;     /* base^2 */
    seq_t t2 = t1 * base;       /* base^3 */
    seq_t t3 = t2 * base;       /* base^4 */
    seq_t t4 = t3 * base;       /* base^5 */
    seq_t t5 = t4 * base;       /* base^6 */

    return t5;
}
