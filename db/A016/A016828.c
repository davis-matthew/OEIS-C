/*
 * A016828  a(n) = (4*n+2)^4.
 * https://oeis.org/A016828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+2)^4
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 63, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)4 * n + 2;   /* (4*n+2) */
    seq_t v2 = v * v;             /* (4*n+2)^2 */
    seq_t v4 = v2 * v2;           /* (4*n+2)^4 */
    return v4;
}
