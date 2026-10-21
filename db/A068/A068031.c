/*
 * A068031  Number of subsets of {1,2,3,...,n} that sum to 0 mod 10.
 * https://oeis.org/A068031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,n} with sum ≡ 0 (mod 10).
 *            Computation: dynamic programming over residues mod 10
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* dp[r] = number of subsets processed so far with sum ≡ r (mod 10) */
    seq_t dp[10];
    for (int r = 0; r < 10; ++r)
        dp[r] = (seq_t)0;
    dp[0] = (seq_t)1;                 /* empty subset */

    seq_t old[10];                     /* snapshot of dp before each element */
    for (idx_t x = 1; x <= n; ++x)
    {
        /* copy current DP values to old */
        for (int r = 0; r < 10; ++r)
            old[r] = dp[r];

        int shift = (int)(x % (idx_t)10);   /* residue of x modulo 10 */

        /* add contributions of subsets that include element x */
        for (int r = 0; r < 10; ++r)
        {
            int newr = (r + shift) % 10;
            dp[newr] = dp[newr] + old[r];
        }
    }
    return dp[0];                      /* subsets summing to 0 mod 10 */
}
