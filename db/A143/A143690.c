/*
 * A143690  a(n) = A007318 * [1, 6, 14, 9, 0, 0, 0, ...].
 * https://oeis.org/A143690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(3*n^2 + 2*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;             /* convert index to sequence type */
    seq_t n2 = nn * nn;              /* n^2  */
    seq_t t  = 3 * n2 + 2 * nn + 2;  /* 3*n^2 + 2*n + 2  */
    seq_t res = (nn + 1) * t / 2;    /* (n+1)*t / 2  */
    return res;
}
