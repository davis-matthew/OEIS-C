/*
 * A126678  Product_{i=5..n} |Stirling_1(i,5)|.
 * https://oeis.org/A126678
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{i=5}^{n} |Stirling_1(i,5)| (unsigned Stirling
 *            numbers of the first kind)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence for unsigned Stirling numbers:
   c(n,k) = c(n-1,k-1) + (n-1)*c(n-1,k)  with c(0,0)=1   */

/* No external dependencies except the types seq_t and idx_t which are supplied by the build. */
seq_t a(idx_t n)
{
    /* For indices below the offset, behavior is irrelevant. Return 1 as a safe default. */
    if (n < 5)
        return (seq_t)1;

    /* c[0..5] will hold the unsigned Stirling numbers c(current_n, k). */
    seq_t c[6] = {0};
    c[0] = 1;

    seq_t prod = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Update in descending order to avoid overwriting needed values. */
        for (int k = 5; k >= 1; --k)
        {
            c[k] = c[k-1] + ((seq_t)(i - 1)) * c[k];
        }
        c[0] = 0;   /* c(n,0) = 0 for n>0 */

        if (i >= 5)
            prod = prod * c[5];
    }

    return prod;
}
