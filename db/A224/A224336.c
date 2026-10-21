/*
 * A224336  Number of idempotent 5 X 5 0..n matrices of rank 4.
 * https://oeis.org/A224336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^4 + 40*n^3 + 60*n^2 + 40*n + 5
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 142, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 10*nn + 40;   /* 10*n + 40 */
    val = val*nn + 60;        /* (10*n + 40)*n + 60  */
    val = val*nn + 40;        /* ((10*n + 40)*n + 60)*n + 40 */
    val = val*nn + 5;         /* (((10*n + 40)*n + 60)*n + 40)*n + 5 */
    return val;
}
