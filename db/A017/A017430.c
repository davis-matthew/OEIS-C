/*
 * A017430  a(n) = (11*n + 3)^6.
 * https://oeis.org/A017430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 147, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t  = 11 * tn + 3;
    seq_t t2 = t * t;      /* t^2 */
    seq_t t4 = t2 * t2;    /* t^4 */
    seq_t t6 = t4 * t2;    /* t^6 = t^4 * t^2 */
    return t6;
}
