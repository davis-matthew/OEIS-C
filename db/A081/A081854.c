/*
 * A081854  a(n) = (8*n - 3)*(4*n - 1)*(8*n^2 - 5*n + 1).
 * https://oeis.org/A081854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n - 3)*(4*n - 1)*(8*n^2 - 5*n + 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    seq_t sn = (seq_t)n;
    seq_t t1 = (seq_t)8 * sn - (seq_t)3;   /* 8n-3   */
    seq_t t2 = (seq_t)4 * sn - (seq_t)1;   /* 4n-1   */
    seq_t t3 = (seq_t)8 * sn * sn - (seq_t)5 * sn + (seq_t)1; /* 8n^2-5n+1 */
    return t1 * t2 * t3;
}
