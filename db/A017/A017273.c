/*
 * A017273  a(n) = (10*n)^5.
 * https://oeis.org/A017273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 8, u64 n <= 713, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t t  = nn * nn;           /* n² */
    t = t * nn;                   /* n³ */
    t = t * nn;                   /* n⁴ */
    t = t * nn;                   /* n⁵ */
    t = t * (seq_t)100000;        /* multiply by 10⁵ */
    return t;
}
