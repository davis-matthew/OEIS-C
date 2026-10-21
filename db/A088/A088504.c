/*
 * A088504  Sum of even entries in row n of Pascal's triangle.
 * https://oeis.org/A088504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n} C(n,k) over those k for which C(n,k) is
 *            even
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 1;          /* C(n,0) */
    seq_t even_sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* add to sum if the current binomial coefficient is even */
        if ((cur & 1) == 0)
            even_sum += cur;

        /* compute next binomial C(n,k+1)   (stop after k==n) */
        if (k < n)
        {
            /* cur = cur * (n - k) / (k + 1);   all in seq_t arithmetic   */
            cur = (cur * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }
    return even_sum;
}
