/*
 * A017069  a(n) = (8*n)^5.
 * https://oeis.org/A017069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 10, u64 n <= 891,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v  = (seq_t)n * 8;   /* 8*n */
    seq_t v2 = v * v;         /* (8*n)^2 */
    seq_t v4 = v2 * v2;       /* (8*n)^4 */
    return v4 * v;            /* (8*n)^5 */
}
