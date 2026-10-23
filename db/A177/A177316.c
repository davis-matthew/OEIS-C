/*
 * A177316  Number of permutations of n copies of 1..4 with all adjacent
 *          differences <= 1 in absolute value.
 * https://oeis.org/A177316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(n+k-1,k)^2 (OEIS
 *            A177316)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom1 = 1; /* binomial(n, k) */
    seq_t binom2 = 1; /* binomial(n+k-1, k) */
    idx_t k;

    for (k = 0; k <= n; ++k)
    {
        if (k > 0)
        {
            /* update binomial(n, k) from binomial(n, k-1) */
            binom1 = binom1 * ((seq_t)(n - (k - 1))) / (seq_t)k;
            /* update binomial(n+k-1, k) from binomial(n+k-2, k-1) */
            binom2 = binom2 * ((seq_t)(n + k - 1)) / (seq_t)k;
        }

        seq_t t1 = binom1 * binom1;   /* binomial(n, k)^2 */
        seq_t t2 = binom2 * binom2;   /* binomial(n+k-1, k)^2 */
        sum += t1 * t2;
    }

    return sum;
}
