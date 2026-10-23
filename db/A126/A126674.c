/*
 * A126674  a(n) = n!*Sum_{j=0..n-1} 2^j/(j+1).
 * https://oeis.org/A126674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{j=0..n-1} 2^j/(j+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t factorial = 1;   /* (i‑1)!  starts as 0! = 1 */
    seq_t pow2     = 1;    /* 2^(i‑1)  starts as 2^0 = 1 */
    seq_t ans      = 0;    /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* a(i) = i * a(i-1) + 2^(i-1) * (i-1)! */
        seq_t term = (seq_t)i * ans + pow2 * factorial;
        ans = term;

        /* prepare for next iteration */
        factorial *= (seq_t)i;   /* factorial becomes i!   */
        if (i < n)               /* only needed if loop continues */
            pow2 <<= 1;          /* pow2 becomes 2^i       */
    }
    return ans;
}
