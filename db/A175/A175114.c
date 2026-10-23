/*
 * A175114  First differences of A175113.
 * https://oeis.org/A175114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n*(12*n^2+1)*(4*n^2+3) ; a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t tn  = (seq_t)n;
    seq_t n2  = tn * tn;
    seq_t t1  = 12 * n2 + (seq_t)1;   /* 12*n^2 + 1 */
    seq_t t2  = 4  * n2 + (seq_t)3;   /* 4*n^2 + 3  */
    seq_t prod = t1 * tn;             /* (12*n^2+1)*n  */
    prod = prod * t2;                 /* * (4*n^2+3)   */
    return prod * (seq_t)4;           /* final factor 4 */
}
