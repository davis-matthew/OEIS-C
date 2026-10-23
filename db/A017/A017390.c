/*
 * A017390  a(n) = (11*n)^2.
 * https://oeis.org/A017390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n)^2
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 5957, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;   /* convert index to the integer type of the result */
    t *= (seq_t)11;       /* compute 11*n */
    return t * t;         /* square the result */
}
