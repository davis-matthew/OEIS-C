/*
 * A069075  a(n) = (4*n^2 - 1)^2.
 * https://oeis.org/A069075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n^2 - 1)^2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n;   /* convert index to seq_t */
    seq_t v   = t * t;      /* n^2          */
    v = v * 4;              /* 4*n^2        */
    v = v - 1;              /* 4*n^2 - 1    */
    v = v * v;              /* (4*n^2 - 1)^2 */
    return v;
}
