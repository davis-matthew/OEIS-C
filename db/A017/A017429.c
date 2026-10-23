/*
 * A017429  a(n) = (11*n+3)^5.
 * https://oeis.org/A017429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n+3)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 7, u64 n <= 648, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = 11 * (seq_t)n + 3;  /* base value = 11*n + 3 */
    seq_t t2 = t * t;              /* (11*n+3)^2 */
    seq_t t4 = t2 * t2;            /* (11*n+3)^4 */
    return t4 * t;                 /* (11*n+3)^5 */
}
