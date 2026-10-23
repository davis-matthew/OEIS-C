/*
 * A171734  First differences of A171733.
 * https://oeis.org/A171734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 4*n+2, a(2n+1) = 4*n*(n+1)*(n+2)
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1u) != 0) {                 /* n is odd, n = 2*k+1 */
        idx_t k = n >> 1;                /* k = floor(n/2) = (n-1)/2 */
        seq_t kk = (seq_t)k;
        seq_t val = kk * (kk + 1);       /* kk*(kk+1)   */
        val = val * (kk + 2);            /* * (kk+2)    */
        val = val * 4;                   /* * 4         */
        return val;
    } else {                              /* n is even, n = 2*k */
        idx_t k = n >> 1;                /* k = n/2     */
        return ((seq_t)4 * (seq_t)k + 2);
    }
}
