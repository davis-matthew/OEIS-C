/*
 * A016996  a(n) = (7*n + 1)^4.
 * https://oeis.org/A016996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 36, u64 n <= 9362,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 7 + 1;   /* t = 7*n + 1 */
    seq_t t2 = t * t;             /* t^2 */
    return t2 * t2;               /* (t^2)^2 = t^4 */
}
