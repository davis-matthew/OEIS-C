/*
 * A017296  a(n) = (10*n + 2)^4.
 * https://oeis.org/A017296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 25, u64 n <= 6553,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * 10 + 2;          /* t = 10*n + 2 */
    return t * t * t * t;    /* (10*n + 2)^4 */
}
