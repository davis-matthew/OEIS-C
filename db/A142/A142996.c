/*
 * A142996  a(0) = 0, a(1) = 1, a(n+1) = (2*n^2+2*n+7)*a(n) - n^4*a(n-1), n >=
 *          1.
 * https://oeis.org/A142996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n+1) = (2*n^2 + 2*n + 7)*a(n) -
 *            n^4*a(n-1), n >= 1. Implements it iteratively; works for any
 *            seq_t/idx_t types provided by the build
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;           /* a(0) */
    seq_t prev1 = (seq_t)1;           /* a(1) */

    for (idx_t k = (idx_t)1; k < n; ++k) {
        seq_t kk   = (seq_t)k;                /* k */
        seq_t kk2  = kk * kk;                /* k^2 */
        seq_t factor = (seq_t)2 * kk2 + (seq_t)2 * kk + (seq_t)7;  /* 2*k^2+2*k+7 */
        seq_t term1 = factor * prev1;        /* (2*k^2+2*k+7)*a(k) */
        seq_t kk4   = kk2 * kk2;             /* k^4 */
        seq_t term2 = kk4 * prev2;           /* k^4*a(k-1) */
        seq_t cur   = term1 - term2;        /* a(k+1) */

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;   /* a(n) */
}
