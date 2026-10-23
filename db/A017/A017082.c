/*
 * A017082  a(n) = (8*n + 1)^6.
 * https://oeis.org/A017082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 203, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 8*n + 1 */
    seq_t base = (seq_t)8 * (seq_t)n + (seq_t)1;

    /* compute power by repeated multiplication */
    seq_t p2 = base * base;   /* base^2 */
    seq_t p4 = p2 * p2;       /* base^4 */
    seq_t p6 = p4 * p2;       /* base^6 */

    return p6;
}
