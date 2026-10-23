/*
 * A017094  a(n) = (8*n + 2)^6.
 * https://oeis.org/A017094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 202, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n * 8 + 2;   /* 8*n + 2   */
    seq_t x2 = x * x;             /* (8n+2)^2  */
    seq_t x4 = x2 * x2;           /* (8n+2)^4  */
    return x4 * x2;               /* (8n+2)^6  */
}
