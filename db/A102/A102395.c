/*
 * A102395  A mod 2 related Jacobsthal sequence.
 * https://oeis.org/A102395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} [ (n+k) mod 3 == 0 ] * (C(n,k) mod 2) Computed
 *            as the number of subsets of the set bits of n whose sum of
 *            powers of two is congruent to (-n) mod 3
 * Range:     u8 n <= 1022, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t dp[3];
    dp[0] = 1;
    dp[1] = 0;
    dp[2] = 0;

    idx_t i = 0;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            int val = (i & 1) ? 2 : 1;  /* 2^i mod 3 */
            seq_t old0 = dp[0];
            seq_t old1 = dp[1];
            seq_t old2 = dp[2];
            if (val == 1) {
                dp[0] = old0 + old2;
                dp[1] = old1 + old0;
                dp[2] = old2 + old1;
            } else { /* val == 2 */
                dp[0] = old0 + old1;
                dp[1] = old1 + old2;
                dp[2] = old2 + old0;
            }
        }
        tmp >>= 1;
        ++i;
    }

    idx_t nmod3 = n % 3;
    int rem = (3 - nmod3) % 3;
    return dp[rem];
}
