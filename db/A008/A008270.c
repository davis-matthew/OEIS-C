/*
 * A008270  Total length of strings on n symbols in Stockhausen problem.
 * https://oeis.org/A008270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0..n-1} binomial(n-1,k) * (2*k+1) * (2*k+1)! /
 *            2^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t sum   = 0;
    seq_t binom = 1;   /* binomial(n-1, 0) */
    seq_t D     = 1;   /* D_k = (2k+1)! / 2^k,  D_0 = 1 */

    for (idx_t k = 0; k <= n - 1; ++k)
    {
        seq_t term = binom;
        term *= (seq_t)(2 * k + 1);
        term *= D;
        sum += term;

        if (k < n - 1)
        {
            /* Update D for next k: D_{k+1} = D_k * (k+1) * (2k+3) */
            D *= (seq_t)(k + 1);
            D *= (seq_t)(2 * k + 3);

            /* Update binom for next k: C_{k+1} = C_k * (n-1-k) / (k+1) */
            binom *= (seq_t)(n - 1 - k);
            binom /= (seq_t)(k + 1);
        }
    }

    return (seq_t)n * sum;
}
