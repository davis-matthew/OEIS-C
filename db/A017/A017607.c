/*
 * A017607  a(n) = (12*n + 7)^3.
 * https://oeis.org/A017607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 7)^3
 * Range:     u8 none, u16 n <= 2, u32 n <= 134, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;       /* convert index to seq_t */
    t = 12 * t + 7;           /* compute 12*n + 7 */
    return t * t * t;         /* cube the result */
}
