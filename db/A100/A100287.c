/*
 * A100287  First occurrence of n in A100002; the least k such that A100002(k)
 *          = n.
 * https://oeis.org/A100287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A101224(n,n) where A101224(n,1) = n^2 - n + 1
 *            A101224(n,k) = (n-k+1) * floor( (A101224(n,k-1)-1)/(n-k+1) )
 * Range:     u8 n <= 16, u16 n <= 276, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset 1: a(1) is first term */

    /* initial value: n^2 - n + 1 */
    seq_t val = (seq_t)n * (seq_t)n - (seq_t)n + 1;

    /* successive floors: divisor decreases from n-1 down to 1 */
    for (idx_t d = n - 1; d > 0; --d)
    {
        /* val = d * floor((val - 1) / d) */
        val = (seq_t)d * ((val - 1) / d);
    }

    return val;
}
