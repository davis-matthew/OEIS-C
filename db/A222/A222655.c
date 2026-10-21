/*
 * A222655  a(n) = 16*n^4 + 4.
 * https://oeis.org/A222655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^4 + 4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn;   /* n^2 */
    t = t * t;           /* n^4 */
    t = t * 16;          /* 16*n^4 */
    return t + 4;        /* +4 */
}
