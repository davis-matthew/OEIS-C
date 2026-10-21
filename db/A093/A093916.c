/*
 * A093916  a(2*k-1) = (2*k-1)^2 + 2 - k, a(2*k) = 6*k^2 + 2 - k: First column
 *          of the triangle A093915.
 * https://oeis.org/A093916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n odd) ? n*n + 2 - ((n+1)/2) : 6*((n/2)^2) + 2 - (n/2)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 1, but safe fallback */

    if (n & 1)          /* n is odd */
    {
        idx_t k = (n + 1) >> 1;          /* k = (n+1)/2 */
        seq_t nn = (seq_t)n;
        seq_t kk = (seq_t)k;
        return nn * nn + (seq_t)2 - kk;
    }
    else                /* n is even */
    {
        idx_t k = n >> 1;                 /* k = n/2 */
        seq_t kk = (seq_t)k;
        return (seq_t)6 * kk * kk + (seq_t)2 - kk;
    }
}
